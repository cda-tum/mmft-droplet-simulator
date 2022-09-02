#include "MergeBifurcationEvent.h"

#include "Droplet.h"
#include "DropletBoundary.h"
#include "Event.h"
#include "Simulation.h"

namespace sim {

MergeBifurcationEvent::MergeBifurcationEvent(double time, Droplet& droplet0, DropletBoundary& boundary0, Droplet& droplet1, Simulation& simulation) : Event(time, 0), droplet0(droplet0), boundary0(boundary0), droplet1(droplet1), simulation(simulation) {}

void MergeBifurcationEvent::performEvent() {
    auto newDroplet = simulation.mergeDroplets(droplet0.getId(), droplet1.getId());

    // add boundaries from droplet0
    for (auto& boundary : droplet0.getBoundaries()) {
        // do not add boundary to new droplet
        if (boundary.get() == &boundary0) {
            continue;
        }

        // add boundary
        newDroplet->addBoundary(boundary->getChannelPosition().getChannel(), boundary->getChannelPosition().getPosition(), boundary->isVolumeTowardsNode0(), boundary->getState());
    }

    // add fully occupied channels from droplet0
    for (auto& channel : droplet0.getFullyOccupiedChannels()) {
        newDroplet->addFullyOccupiedChannel(channel);
    }

    // add boundaries from droplet1
    for (auto& boundary : droplet1.getBoundaries()) {
        newDroplet->addBoundary(boundary->getChannelPosition().getChannel(), boundary->getChannelPosition().getPosition(), boundary->isVolumeTowardsNode0(), boundary->getState());
    }

    // add fully occupied channels from droplet1
    for (auto& channel : droplet1.getFullyOccupiedChannels()) {
        newDroplet->addFullyOccupiedChannel(channel);
    }

    // check if droplet0 is inside a single channel, because if not then also a fully occupied channel has to be added
    if (!droplet0.isInsideSingleChannel()) {
        newDroplet->addFullyOccupiedChannel(boundary0.getChannelPosition().getChannel());
    }

    // add/remove droplets form network
    newDroplet->setDropletState(DropletState::NETWORK);
    droplet0.setDropletState(DropletState::SINK);
    droplet1.setDropletState(DropletState::SINK);
}

}  // namespace sim