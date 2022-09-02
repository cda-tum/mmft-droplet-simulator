#include "Droplet.h"

#include <cmath>
#include <memory>
#include <string>

#include "Channel.h"
#include "IResistanceModel.h"

namespace sim {

Droplet::Droplet(int id, double volume, Fluid* fluid) : id(id), volume(volume), fluid(fluid) {}

void Droplet::setVolume(double volume) {
    this->volume = volume;
}

void Droplet::setName(std::string name) {
    this->name = std::move(name);
}

void Droplet::setDropletState(DropletState dropletState) {
    this->dropletState = dropletState;
}

int Droplet::getId() const {
    return id;
}

std::string Droplet::getName() const {
    return name;
}

double Droplet::getVolume() const {
    return volume;
}

DropletState Droplet::getDropletState() const {
    return dropletState;
}

const Fluid* Droplet::getFluid() const {
    return fluid;
}

void Droplet::addResistances(const IResistanceModel& model) {
    // check if droplet is in a single channel
    if (isInsideSingleChannel()) {
        auto channel = boundaries[0]->getChannelPosition().getChannel();
        // volumeInsideChannel = volumeChannel - (volumeBoundary0 - volumeChannel) - (volumeBoundary1 - volumeChannel) = volumeBoundary0 + volumeBoundary1 - volumeChannel
        double volumeInsideChannel = boundaries[0]->getVolume() + boundaries[1]->getVolume() - channel->getVolume();
        channel->addDropletResistance(model.getDropletResistance(channel, this, volumeInsideChannel));
    } else {
        // loop through boundaries
        for (auto& boundary : boundaries) {
            auto channel = boundary->getChannelPosition().getChannel();
            channel->addDropletResistance(model.getDropletResistance(channel, this, boundary->getVolume()));
        }

        // loop through fully occupied channels (if present)
        for (auto& channel : channels) {
            channel->addDropletResistance(model.getDropletResistance(channel, this, channel->getVolume()));
        }
    }
}

const std::vector<std::unique_ptr<sim::DropletBoundary>>& Droplet::getBoundaries() const {
    return boundaries;
}
std::vector<arch::Channel*>& Droplet::getFullyOccupiedChannels() {
    return channels;
}

bool Droplet::isAtBifurcation() {
    // TODO: maybe refine the definition, so it is only true if no channel branches of within the droplet range (then needs a chip reference as input)
    // right now it is true if the droplet is not in a single channel
    return !isInsideSingleChannel();
}

bool Droplet::isInsideSingleChannel() {
    // is inside a single channel when only two boundaries are present and they are in the same channel
    return channels.size() == 0 && boundaries.size() == 2 && boundaries[0]->getChannelPosition().getChannel() == boundaries[1]->getChannelPosition().getChannel();
}

void Droplet::addBoundary(arch::Channel* channel, double position, bool volumeTowardsNode0, BoundaryState state) {
    boundaries.push_back(std::make_unique<DropletBoundary>(channel, position, volumeTowardsNode0, state));
}

void Droplet::addFullyOccupiedChannel(arch::Channel* channel) {
    channels.push_back(channel);
}

void Droplet::removeBoundary(DropletBoundary& boundaryReference) {
    // TODO: remove more than one boundary at once (remove_if)

    for (int i = 0; i < boundaries.size(); i++) {
        if (boundaries[i].get() == &boundaryReference) {
            boundaries.erase(boundaries.begin() + i);
            break;
        }
    }
}

void Droplet::removeFullyOccupiedChannel(int channelId) {
    for (int i = 0; i < channels.size(); i++) {
        if (channels[i]->getId() == channelId) {
            channels.erase(channels.begin() + i);
            break;
        }
    }
}

std::vector<DropletBoundary*> Droplet::getConnectedBoundaries(int nodeId, DropletBoundary* doNotConsider) {
    std::vector<DropletBoundary*> connectedBoundaries;
    for (auto& boundary : boundaries) {
        // do not consider boundary
        if (boundary.get() == doNotConsider) {
            continue;
        }

        if (boundary->getReferenceNode()->getId() == nodeId) {
            connectedBoundaries.push_back(boundary.get());
        }
    }

    return connectedBoundaries;
}

std::vector<arch::Channel*> Droplet::getConnectedFullyOccupiedChannels(int nodeId) {
    std::vector<arch::Channel*> connectedChannels;
    for (auto& channel : channels) {
        if (nodeId == channel->getNode0()->getId() || nodeId == channel->getNode1()->getId()) {
            connectedChannels.push_back(channel);
        }
    }

    return connectedChannels;
}

void Droplet::updateBoundaries(const arch::Chip& chip, double slipFactor) {
    // compute the average flow rates of all boundaries, since the inflow does not necessarily have to match the outflow (qInput != qOutput)
    // in order to avoid an unwanted increase/decrease of the droplet volume an average flow rate is computed
    // the actual flow rate of a boundary is then determined accordingly to the ratios of the different flowRates inside the channels

    // determine the state of the droplet
    double qInflow = 0;
    double qOutflow = 0;
    std::vector<DropletBoundary*> outflowBoundaries;
    std::vector<DropletBoundary*> inflowBoundaries;

    // loop through boundaries
    for (auto& boundary : boundaries) {
        // a boundary can stop at a bifurcation, e.g., when all flow rates of the connected channels have an inflow (point to the center of the boundary), or when only a bypass channel is present
        // when this happens it usually goes into a WaitOutflow state where the flow rate of the boundary becomes zero
        // here we have to check again, if this state still holds, or if the boundary can flow normally again
        boundary->updateWaitState(chip);

        // if the boundary is still in a Wait state, then do not add it to the computation of the inflow/outflow
        if (boundary->isInWaitState()) {
            continue;
        }

        // this flow rate is already oriented in such a way, that a negative value indicates an inflow and a positive value an outflow
        // inflow => boundary moves towards the droplet center
        // outflow => boundary moves away from the droplet center
        double flowRate = boundary->getChannelFlowRate();

        if (flowRate < 0) {
            // inflow occurs
            inflowBoundaries.push_back(boundary.get());
            qInflow += -flowRate;  // only the absolute value of qInflow is important, hence, the minus sign
        } else if (flowRate > 0) {
            // outflow occurs
            outflowBoundaries.push_back(boundary.get());
            qOutflow += flowRate;
        } else {
            // flow rate is zero and, thus, does not contribute to the in/outflow
        }
    }

    if (inflowBoundaries.size() == 0 && outflowBoundaries.size() != 0) {
        // only outflow and no inflow occurs
        // this scenario is not supported yet and just stops the movement of all boundaries of this droplet
        // possible solution would be to split the droplet, or that slower boundaries get dragged along faster ones

        // just print a warning
        std::cout << "WARNING: All boundaries of droplet (id=" << id << ") move away from the center of the droplet. Droplet volume conservation cannot be guaranteed, hence the droplet movement is stopped." << std::endl;

        for (auto boundary : outflowBoundaries) {
            boundary->setFlowRate(0);
        }
    } else if (inflowBoundaries.size() != 0 && outflowBoundaries.size() == 0) {
        // only inflow and no outflow occurs
        // this scenario is not supported yet and just stops the movement of all boundaries of this droplet

        // just print a warning
        std::cout << "WARNING: All boundaries of droplet (id=" << id << ") move towards the center of the droplet. Droplet volume conservation cannot be guaranteed, hence the droplet movement is stopped." << std::endl;

        for (auto boundary : inflowBoundaries) {
            boundary->setFlowRate(0);
        }
    } else if (inflowBoundaries.size() != 0 && outflowBoundaries.size() != 0) {
        // outflow and inflow occurs

        // determine the average flow rate (important, when inflow and outflow are not the same for example due to a bypass channel)
        auto qAverage = (qInflow + qOutflow) / 2;

        // outflow boundaries
        for (auto boundary : outflowBoundaries) {
            //consider the slipFactor (droplet is faster than the continuous phase)
            boundary->setFlowRate(slipFactor * qAverage * boundary->getChannelFlowRate() / qOutflow);
        }

        // inflow boundaries
        for (auto boundary : inflowBoundaries) {
            //consider the slipFactor (droplet is faster than the continuous phase)
            boundary->setFlowRate(slipFactor * qAverage * boundary->getChannelFlowRate() / qInflow);
        }
    } else {
        // should not happen
    }
}

void Droplet::addMergedDroplet(Droplet* droplet) {
    mergedDroplets.push_back(droplet);
}

const std::vector<Droplet*>& Droplet::getMergedDroplets() const {
    return mergedDroplets;
}

}  // namespace sim