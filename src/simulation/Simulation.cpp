#include "Simulation.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "BoundaryHeadEvent.h"
#include "BoundaryTailEvent.h"
#include "Channel.h"
#include "ChannelPosition.h"
#include "Droplet.h"
#include "DropletBoundary.h"
#include "IFlowRatePump.h"
#include "INode.h"
#include "IPressurePump.h"
#include "IResistance.h"
#include "Injection.h"
#include "InjectionEvent.h"
#include "MergeBifurcationEvent.h"
#include "MergeChannelEvent.h"
#include "NodalAnalysis.h"
#include "ResistanceModels.h"
#include "Results.h"
#include "TimeStepEvent.h"

namespace sim {

Simulation::Simulation() {}

void Simulation::setChip(arch::Chip* chip) {
    this->chip = chip;
}

arch::Chip* Simulation::getChip() {
    return chip;
}

void Simulation::setContinuousPhase(int fluidId) {
    continuousPhase = getFluid(fluidId);
}

void Simulation::setResistanceModel(ResistanceModel modelName) {
    this->resistanceModelName = modelName;
}

void Simulation::setMaximalAdaptiveTimeStep(double timeStep) {
    maximalAdaptiveTimeStep = timeStep;
}

Fluid* Simulation::addFluid(double viscosity, double density, double concentration) {
    auto id = fluids.size();

    auto result = fluids.insert_or_assign(id, std::make_unique<Fluid>(id, viscosity, density, concentration));

    return result.first->second.get();
}

Fluid* Simulation::getFluid(int fluidId) {
    return fluids.at(fluidId).get();
}

Droplet* Simulation::addDroplet(int fluidId, double volume) {
    auto id = droplets.size();
    auto fluid = fluids.at(fluidId).get();

    auto result = droplets.insert_or_assign(id, std::make_unique<Droplet>(id, volume, fluid));

    return result.first->second.get();
}

Droplet* Simulation::getDroplet(int dropletId) {
    return droplets.at(dropletId).get();
}

Droplet* Simulation::getDropletAtNode(int nodeId) {
    // loop through all droplets
    for (auto& [id, droplet] : droplets) {
        // do not consider droplets which are not inside the network
        if (droplet->getDropletState() != DropletState::NETWORK) {
            continue;
        }

        // do not consider droplets inside a single channel (because they cannot span over a node)
        if (droplet->isInsideSingleChannel()) {
            continue;
        }

        // check if a boundary is connected with the reference node and if yes then return the droplet immediately
        auto connectedBoundaries = droplet->getConnectedBoundaries(nodeId);
        if (connectedBoundaries.size() != 0) {
            return droplet.get();
        }

        // check if a fully occupied channel is connected with the reference node and if yes then return the droplet immediately
        auto connectedFullyOccupiedChannels = droplet->getConnectedFullyOccupiedChannels(nodeId);
        if (connectedFullyOccupiedChannels.size() != 0) {
            return droplet.get();
        }
    }

    // if nothing was found than return nullptr
    return nullptr;
}

Injection* Simulation::addInjection(int dropletId, double injectionTime, int channelId, double injectionPosition) {
    auto id = injections.size();
    auto droplet = droplets.at(dropletId).get();
    auto channel = chip->getChannel(channelId);

    // --- check if injection is valid ---
    // for the injection the head and tail of the droplet must lie inside the channel (the volume of the droplet must be small enough)
    // the droplet length is a relative value between 0 and 1
    double dropletLength = droplet->getVolume() / channel->getVolume();
    // channel must be able to fully contain the droplet
    if (dropletLength >= 1.0) {
        throw std::invalid_argument("Injection of droplet " + droplet->getName() + " into channel " + channel->getName() + " is not valid. Channel must be able to fully contain the droplet.");
    }
    // compute tail and head position of the droplet
    double tail = (injectionPosition - dropletLength / 2);
    double head = (injectionPosition + dropletLength / 2);
    // tail and head must not be outside the channel (can happen when the droplet is injected at the beginning or end of the channel)
    if (tail < 0 || head > 1.0) {
        throw std::invalid_argument("Injection of droplet " + droplet->getName() + " is not valid. Tail and head of the droplet must lie inside the channel " + channel->getName() + ". Consider to set the injection position in the middle of the channel.");
    }

    auto result = injections.insert_or_assign(id, std::make_unique<Injection>(id, droplet, injectionTime, channel, injectionPosition));

    return result.first->second.get();
}

Injection* Simulation::getInjection(int injectionId) {
    return injections.at(injectionId).get();
}

droplet::SimulationResult Simulation::simulate() {
    // ##########
    // Initialize
    // ##########

    droplet::SimulationResult result;
    storeSimulationParameters(result);

    // initialize the simulation
    initialize();

    // get nodes, channels, pumps
    // nodes (<nodeId, <Node, matrixId>>): key is the nodeId and the tuple consists of the node and the correspondng matrixId (the groundNode has matrixId -1)
    // this is done, because the nodal analysis needs a nodeId that represents the node's index in the matrix (hence, the matrixId), which is usually not the same as the nodeId
    // the node itself inside the tuple is not really necessary, but is convenient when writing all pressure values to each node, after solving the matrix
    std::unordered_map<int, std::tuple<nodal::INode*, int>> nodes;
    int matrixId = 0;
    for (auto& [nodeId, node] : chip->getNodes()) {
        // if node is ground node set the matrixId to -1
        if (nodeId == chip->getGroundId()) {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), -1));
        } else {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), matrixId));
            matrixId++;
        }
    }
    std::vector<arch::Channel*> channels;
    for (const auto& c: chip->getChannels())
        channels.push_back (c.second.get());
    std::vector<arch::FlowRatePump*> flowRatePumps;
    for (const auto& p: chip->getFlowRatePumps())
        flowRatePumps.push_back (p.second.get());
    std::vector<arch::PressurePump*> pressurePumps;
    for (const auto& p: chip->getPressurePumps())
        pressurePumps.push_back (p.second.get());

    // ##########
    // Simulation Loop
    // ##########
    // * update droplet resistances
    // * conduct nodal analysis
    // * update droplets (flow rates of boundaries)
    // * compute events
    // * search for next event (break if no event is left)
    // * move droplets
    // * perform event
    while (true) {
        // update droplet resistances (in the first iteration no  droplets are inside the network)
        updateDropletResistances();

        // compute nodal analysis
        nodal::conductNodalAnalysis(nodes, channels, pressurePumps, flowRatePumps);

        // update droplets, i.e., their boundary flow rates
        // loop over all droplets
        dropletsAtBifurcation = false;
        for (auto& [key, droplet] : droplets) {
            // only consider droplets inside the network
            if (droplet->getDropletState() != DropletState::NETWORK) {
                continue;
            }

            // set to true if droplet is at bifurcation
            if (droplet->isAtBifurcation()) {
                dropletsAtBifurcation = true;
            }

            // compute the average flow rates of all boundaries, since the inflow does not necessarily have to match the outflow (qInput != qOutput)
            // in order to avoid an unwanted increase/decrease of the droplet volume an average flow rate is computed
            // the actual flow rate of a boundary is then determined accordingly to the ratios of the different flowRates inside the channels
            droplet->updateBoundaries(*chip, slipFactor);
        }

        // merging:
        // 1) Two boundaries merge inside a single channel:
        //      1) one boundary is faster than the other and would "overtake" the boundary
        //          * theoretically it would also be possible to introduce a merge distance.
        //            however, then it gets complicated when the actual merging of the droplets happen, since all other boundaries of the droplets need to moved in order to conserve the droplet volume (i.e., compensate for the volume between the droplets due to the merge distance).
        //            this movement of all other boundaries would then may trigger other events => it gets complicated^^
        //      2) the two boundaries flow in different directions:
        //          * this case shouldn't happen in this implementation now, but it might be possible since boundary flow rates are not bound to channel flow rates.
        //            however, currently the boundaries should still have the same direction as the channel flow rates and, thus, the boundaries cannot have opposite flow directions in a single channel
        // 2) A boundary (which currently operates as a droplet head) can merge into another droplet when it switches channels (basically when a BoundaryHeadEvent occurs):
        //      * possible solution is to normally compute a BoundaryHeadEvent and then check if it is actually a merge event (in case of a merge event the channels don't have to be switched since the boundary is merged with the other droplet)

        // store simulation results of current state
        storeSimulationResults(result);

        // compute events
        auto events = computeEvents();

        // sort events
        // closest events in time with the highest priority come first
        std::sort(events.begin(), events.end(), [](auto& a, auto& b) {
            if (a->getTime() == b->getTime()) {
                return a->getPriority() < b->getPriority();  // ascending order (the lower the priority value, the higher the priority)
            }
            return a->getTime() < b->getTime();  // ascending order
        });

        // get next event or break loop, if no events remain
        Event* nextEvent = nullptr;
        if (events.size() != 0) {
            nextEvent = events[0].get();
        } else {
            break;
        }

        // move droplets until event is reached
        currTime += nextEvent->getTime();
        moveDroplets(nextEvent->getTime());

        // perform event (inject droplet, move droplet to next channel, block channel, merge channels)
        // only one event at a time is performed in order to ensure a correct state
        // hence, it might happen that the next time for an event is 0 (e.g., when  multiple events happen at the same time)
        nextEvent->performEvent();
    }

    return result;
}

void Simulation::initialize() {
    // set resistance model
    if (resistanceModelName == ResistanceModel::ONE_D_MODEL) {
        resistanceModel = std::make_unique<ResistanceModel0>(continuousPhase->getViscosity());
    } else if (resistanceModelName == ResistanceModel::TEST_MODEL) {
        resistanceModel = std::make_unique<ResistanceModel1>();
    }

    // compute channel resistances
    for (auto& [key, channel] : chip->getChannels()) {
        double resistance = resistanceModel->getChannelResistance(channel.get());
        channel->setChannelResistance(resistance);
        channel->setDropletResistance(0.0);
    }
}

void Simulation::updateDropletResistances() {
    // set all droplet resistances of all channels to 0.0
    for (auto& [key, channel] : chip->getChannels()) {
        channel->setDropletResistance(0.0);
    }

    // set correct droplet resistances
    for (auto& [key, droplet] : droplets) {
        // only consider droplets that are inside the network (i.e., also trapped droplets)
        if (droplet->getDropletState() == DropletState::INJECTION || droplet->getDropletState() == DropletState::SINK) {
            continue;
        }

        droplet->addResistances(*resistanceModel);
    }
}

std::vector<std::unique_ptr<Event>> Simulation::computeEvents() {
    // events
    std::vector<std::unique_ptr<Event>> events;

    // injection events
    for (auto& [key, injection] : injections) {
        double injectionTime = injection->getInjectionTime();
        if (injection->getDroplet()->getDropletState() == DropletState::INJECTION) {
            events.push_back(std::make_unique<InjectionEvent>(injectionTime - currTime, *injection));
        }
    }

    // define maps that are used for detecting merging inside channels
    std::unordered_map<int, std::vector<DropletBoundary*>> channelBoundariesMap;
    std::unordered_map<DropletBoundary*, Droplet*> boundaryDropletMap;

    for (auto& [key, droplet] : droplets) {
        // only consider droplets inside the network (but no trapped droplets)
        if (droplet->getDropletState() != DropletState::NETWORK) {
            continue;
        }

        // loop through boundaries
        for (auto& boundary : droplet->getBoundaries()) {
            // the flow rate of the boundary indicates if the boundary moves towards or away from the droplet center and, hence, if a BoundaryTailEvent or BoundaryHeadEvent should occur, respectively
            // if the flow rate of the boundary is 0, then no events will be triggered (the boundary may be in a Wait state)
            if (boundary->getFlowRate() < 0) {
                // boundary moves towards the droplet center => BoundaryTailEvent
                double time = boundary->getTime();
                events.push_back(std::make_unique<BoundaryTailEvent>(time, *droplet, *boundary, *chip));
            } else if (boundary->getFlowRate() > 0) {
                // boundary moves away from the droplet center => BoundaryHeadEvent
                double time = boundary->getTime();

                // in this scenario also a MergeBifurcationEvent can happen when merging is enabled
                // this means a boundary comes to a bifurcation where a droplet is already present
                // hence it is either a MergeBifurcationEvent or a BoundaryHeadEvent that will happen

                // check if merging is enabled
                Droplet* mergeDroplet = nullptr;
                if (enableMerging) {
                    // find droplet to merge (if present)
                    auto referenceNode = boundary->getOppositeReferenceNode();
                    mergeDroplet = getDropletAtNode(referenceNode->getId());
                }

                if (mergeDroplet == nullptr) {
                    // no merging will happen => BoundaryHeadEvent
                    events.push_back(std::make_unique<BoundaryHeadEvent>(time, *droplet, *boundary, *chip));
                } else {
                    // merging of the actual droplet with the merge droplet will happen => MergeBifurcationEvent
                    events.push_back(std::make_unique<MergeBifurcationEvent>(time, *droplet, *boundary, *mergeDroplet, *this));
                }
            }

            // fill the maps which are later used for merging inside channels (if merging is enabled)
            if (enableMerging) {
                auto [value, success] = channelBoundariesMap.try_emplace(boundary->getChannelPosition().getChannel()->getId());
                value->second.push_back(boundary.get());

                boundaryDropletMap.emplace(boundary.get(), droplet.get());
            }
        }
    }

    // check for MergeChannelEvents, i.e, for boundaries of other droplets that are in the same channel
    // here the previously defined maps are used => if merging is not enabled these maps are emtpy
    // loop through channelsBoundariesMap
    for (auto& [channelId, boundaries] : channelBoundariesMap) {
        // loop through boundaries that are inside this channel
        for (size_t i = 0; i < boundaries.size(); i++) {
            // get reference boundary and droplet
            auto referenceBoundary = boundaries[i];
            auto referenceDroplet = boundaryDropletMap.at(referenceBoundary);

            // get channel
            auto channel = referenceBoundary->getChannelPosition().getChannel();

            // get velocity and absolute position of the boundary
            // positive values for v0 indicate a movement from node0 towards node1
            auto q0 = referenceBoundary->isVolumeTowardsNode0() ? referenceBoundary->getFlowRate() : -referenceBoundary->getFlowRate();
            auto v0 = q0 / channel->getArea();
            auto p0 = referenceBoundary->getChannelPosition().getPosition() * channel->getLength();

            // compare reference boundary against all others
            // the two loops are defined in such a way, that only one merge event happens for a pair of boundaries
            for (size_t j = i + 1; j < boundaries.size(); j++) {
                auto boundary = boundaries[j];
                auto droplet = boundaryDropletMap.at(boundary);

                // do not consider if this boundary is form the same droplet
                if (droplet == referenceDroplet) {
                    continue;
                }

                // get velocity and absolute position of the boundary
                // positive values for v0 indicate a movement from node0 towards node1
                auto q1 = boundary->isVolumeTowardsNode0() ? boundary->getFlowRate() : -boundary->getFlowRate();
                auto v1 = q1 / channel->getArea();
                auto p1 = boundary->getChannelPosition().getPosition() * channel->getLength();

                // do not merge when both velocities are equal (would result in infinity time)
                if (v0 == v1) {
                    continue;
                }

                // compute time and merge position
                auto time = (p1 - p0) / (v0 - v1);
                auto pMerge = p0 + v0 * time;  // or p1 + v1*time
                auto pMergeRelative = pMerge / channel->getLength();

                // do not trigger a merge event when:
                // * time is negative => indicates that both boundaries go in different directions or that one boundary cannot "outrun" the other because it is too slow
                // * relative merge position is outside the range of [0, 1] => the merging would happen "outside" the channel and a boundary would already switch a channel before this event could happen
                if (time < 0 || pMergeRelative < 0 || 1 < pMergeRelative) {
                    continue;
                }

                // add MergeChannelEvent
                events.push_back(std::make_unique<MergeChannelEvent>(time, *referenceDroplet, *referenceBoundary, *droplet, *boundary, *this));
            }
        }
    }

    // time step event
    if (dropletsAtBifurcation && maximalAdaptiveTimeStep > 0) {
        events.push_back(std::make_unique<TimeStepEvent>(maximalAdaptiveTimeStep));
    }

    return events;
}

void Simulation::moveDroplets(double timeStep) {
    // loop over all droplets
    for (auto& [key, droplet] : droplets) {
        // only consider droplets inside the network (but no trapped droplets)
        if (droplet->getDropletState() != DropletState::NETWORK) {
            continue;
        }

        // loop through boundaries
        for (auto& boundary : droplet->getBoundaries()) {
            // move boundary in correct direction
            boundary->moveBoundary(timeStep);
        }
    }
}

void Simulation::storeSimulationParameters(droplet::SimulationResult& result) {
    // chip name
    result.chip.name = chip->getName();

    // channel
    for (const auto& [key, channel] : chip->getChannels()) {
        result.chip.channels.try_emplace(key, channel->getId(), channel->getName(), channel->getNode0()->getId(), channel->getNode1()->getId(), channel->getWidth(), channel->getHeight(), channel->getLength(), static_cast<droplet::ChannelType>(static_cast<int>(channel->getChannelType())));
    }

    // flow rate pump
    for (const auto& [key, pump] : chip->getFlowRatePumps()) {
        result.chip.flowRatePumps.try_emplace(key, pump->getId(), pump->getName(), pump->getNode0()->getId(), pump->getNode1()->getId(), pump->getFlowRate());
    }

    // pressure pump
    for (const auto& [key, pump] : chip->getPressurePumps()) {
        result.chip.pressurePumps.try_emplace(key, pump->getId(), pump->getName(), pump->getNode0()->getId(), pump->getNode1()->getId(), pump->getPressure());
    }

    // fluids
    for (const auto& [key, fluid] : fluids) {
        auto [value, success] = result.fluids.try_emplace(key, fluid->getId(), fluid->getName(), fluid->getViscosity(), fluid->getDensity(), fluid->getConcentration());

        //mixed fluids
        for (const auto mixedFluid : fluid->getMixedFluids()) {
            value->second.mixedFluidIds.push_back(mixedFluid->getId());
        }
    }

    // droplet
    for (const auto& [key, droplet] : droplets) {
        auto [value, success] = result.droplets.try_emplace(key, droplet->getId(), droplet->getName(), droplet->getVolume(), droplet->getFluid()->getId());

        //merged droplets
        for (const auto mergedDroplet : droplet->getMergedDroplets()) {
            value->second.mergedDropletIds.push_back(mergedDroplet->getId());
        }
    }

    // injections
    for (const auto& [key, injection] : injections) {
        result.injections.try_emplace(key, injection->getId(), injection->getDroplet()->getId(), injection->getInjectionTime(), injection->getInjectionPosition().getChannel()->getId(), injection->getInjectionPosition().getPosition());
    }

    result.continuousPhaseId = continuousPhase->getId();
    result.maximalAdaptiveTimeStep = maximalAdaptiveTimeStep;

    switch (resistanceModelName) {
        case ResistanceModel::ONE_D_MODEL:
            result.resistanceModel = 0;
            break;
        case ResistanceModel::TEST_MODEL:
            result.resistanceModel = 1;
            break;
    }
}

void Simulation::storeSimulationResults(droplet::SimulationResult& result) {
    // add new fluids if present (try_emplace does nothing if key is already present)
    for (const auto& [key, fluid] : fluids) {
        auto [value, success] = result.fluids.try_emplace(key, fluid->getId(), fluid->getName(), fluid->getViscosity(), fluid->getDensity(), fluid->getConcentration());

        //mixed fluids
        if (success) {
            for (const auto mixedFluid : fluid->getMixedFluids()) {
                value->second.mixedFluidIds.push_back(mixedFluid->getId());
            }
        }
    }

    // add new droplets if present (try_emplace does nothing if key is already present)
    for (const auto& [key, droplet] : droplets) {
        auto [value, success] = result.droplets.try_emplace(key, droplet->getId(), droplet->getName(), droplet->getVolume(), droplet->getFluid()->getId());

        //merged droplets
        if (success) {
            for (const auto mergedDroplet : droplet->getMergedDroplets()) {
                value->second.mergedDropletIds.push_back(mergedDroplet->getId());
            }
        }
    }

    // state
    auto& state = result.states.emplace_back(iState++, currTime);

    // pressures
    for (auto& [id, node] : chip->getNodes()) {
        state.pressures.try_emplace(id, node->getPressure());
    }

    // flow rates
    for (auto& [id, channel] : chip->getChannels()) {
        state.flowRates.try_emplace(id, channel->getFlowRate());
    }
    for (auto& [id, pump] : chip->getFlowRatePumps()) {
        state.flowRates.try_emplace(id, pump->getFlowRate());
    }
    for (auto& [id, pump] : chip->getPressurePumps()) {
        state.flowRates.try_emplace(id, pump->getFlowRate());
    }

    // droplet positions
    for (auto& [id, droplet] : droplets) {
        // create new droplet position
        auto [value, success] = state.dropletPositions.try_emplace(id, static_cast<droplet::DropletState>(static_cast<int>(droplet->getDropletState())));

        // add boundaries
        for (auto& boundary : droplet->getBoundaries()) {
            // get channel position
            auto channelPosition = boundary->getChannelPosition();

            // add boundary
            value->second.boundaries.emplace_back(channelPosition.getChannel()->getId(), channelPosition.getPosition(), boundary->isVolumeTowardsNode0(), boundary->getFlowRate(), static_cast<droplet::BoundaryState>(static_cast<int>(boundary->getState())));
        }

        // add fully occupied channels
        for (auto& channel : droplet->getFullyOccupiedChannels()) {
            // add channel
            value->second.channelIds.emplace_back(channel->getId());
        }
    }
}

Fluid* Simulation::mixFluids(int fluid0Id, double volume0, int fluid1Id, double volume1) {
    // check if fluids are identically (no merging needed) and if they exist
    if (fluid0Id == fluid1Id) {
        // try to get the fluid (throws error if the fluid is not present)
        return fluids.at(fluid0Id).get();
    }

    // get fluids
    auto fluid0 = fluids.at(fluid0Id).get();
    auto fluid1 = fluids.at(fluid1Id).get();

    // compute ratios
    double volume = volume0 + volume1;
    double ratio0 = volume0 / volume;
    double ratio1 = volume1 / volume;

    // compute new fluid values
    double viscosity = ratio0 * fluid0->getViscosity() + ratio1 * fluid1->getViscosity();
    double density = ratio0 * fluid0->getDensity() + ratio1 * fluid1->getDensity();
    double concentration = ratio0 * fluid0->getConcentration() + ratio1 * fluid1->getConcentration();

    // add new fluid
    auto newFluid = addFluid(viscosity, density, concentration);

    //add previous fluids
    newFluid->addMixedFluid(fluid0);
    newFluid->addMixedFluid(fluid1);

    return newFluid;
}

Droplet* Simulation::mergeDroplets(int droplet0Id, int droplet1Id) {
    // check if droplets are identically (no merging needed) and if they exist
    if (droplet0Id == droplet1Id) {
        // try to get the droplet (throws error if the droplet is not present)
        return droplets.at(droplet0Id).get();
    }

    // get droplets
    auto droplet0 = getDroplet(droplet0Id);
    auto droplet1 = getDroplet(droplet1Id);

    // compute volumes
    double volume0 = droplet0->getVolume();
    double volume1 = droplet1->getVolume();
    double volume = volume0 + volume1;

    // merge fluids
    auto fluid = mixFluids(droplet0->getFluid()->getId(), volume0, droplet1->getFluid()->getId(), volume1);

    // add new droplet
    auto newDroplet = addDroplet(fluid->getId(), volume);

    //add previous droplets
    newDroplet->addMergedDroplet(droplet0);
    newDroplet->addMergedDroplet(droplet1);

    return newDroplet;
}

}  // namespace sim