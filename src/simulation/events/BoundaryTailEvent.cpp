#include "BoundaryTailEvent.h"

#include <algorithm>
#include <vector>

#include "Channel.h"
#include "Droplet.h"
#include "DropletBoundary.h"
#include "Event.h"
#include "Node.h"

namespace sim {

BoundaryTailEvent::BoundaryTailEvent(double time, Droplet& droplet, DropletBoundary& boundary, const arch::Chip& chip) : Event(time, 1), droplet(droplet), boundary(boundary), chip(chip) {}

void BoundaryTailEvent::performEvent() {
    // get reference node of boundary
    auto referenceNode = boundary.getReferenceNode();

    // get the other boundaries and fully occupied channels inside this droplet that have the same reference node
    auto boundaries = droplet.getConnectedBoundaries(referenceNode->getId(), &boundary);  // do not consider the actual boundary
    auto fullyOccupiedChannels = droplet.getConnectedFullyOccupiedChannels(referenceNode->getId());

    // if more than a single entity (boundary or fully occupied channel) is present, then remove the boundary, otherwise switch the channel
    if (boundaries.size() + fullyOccupiedChannels.size() == 1) {
        // only one entity is present => switch channel

        // get next channel
        auto nextChannel = boundaries.size() == 1 ? boundaries[0]->getChannelPosition().getChannel() : fullyOccupiedChannels[0];

        // get new position (is either 0.0 or 1.0, depending on if node0 or node1 of the nextChannel is the referenceNode)
        double channelPosition = nextChannel->getNode0() == referenceNode ? 0.0 : 1.0;
        bool volumeTowardsNode0 = nextChannel->getNode0() != referenceNode;

        // set new channel, position, direction of volume, and state of the boundary
        boundary.getChannelPosition().setChannel(nextChannel);
        boundary.getChannelPosition().setPosition(channelPosition);
        boundary.setVolumeTowardsNode0(volumeTowardsNode0);
        boundary.setState(BoundaryState::NORMAL);

        // remove fully occupied channel if present
        if (fullyOccupiedChannels.size() == 1) {
            droplet.removeFullyOccupiedChannel(nextChannel->getId());
        }
    } else {
        // more than one entity is present => remove boundary
        droplet.removeBoundary(boundary);
    }
}

}  // namespace sim