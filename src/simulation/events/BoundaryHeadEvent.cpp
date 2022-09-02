#include "BoundaryHeadEvent.h"

#include <memory>
#include <vector>

#include "Channel.h"
#include "Droplet.h"
#include "DropletBoundary.h"
#include "Event.h"
#include "Node.h"

namespace sim {

BoundaryHeadEvent::BoundaryHeadEvent(double time, Droplet& droplet, DropletBoundary& boundary, const arch::Chip& chip) : Event(time, 1), droplet(droplet), boundary(boundary), chip(chip) {}

void BoundaryHeadEvent::performEvent() {
    // get boundary channel
    auto boundaryChannel = boundary.getChannelPosition().getChannel();

    // get correct node (is the opposite node of the boundary reference node inside the channel)
    arch::Node* node = boundary.isVolumeTowardsNode0() ? boundaryChannel->getNode1() : boundaryChannel->getNode0();

    // if this node is a sink then remove the whole droplet from the network
    if ((chip.isSink(node->getId()))) {
        droplet.setDropletState(DropletState::SINK);
        return;
    }

    // get next channels
    std::vector<arch::Channel*> nextChannels = chip.getChannelsAtNode(node->getId());

    // choose branch with the highest instantaneous flow rate
    double maxFlowRate;
    arch::Channel* nextChannel = nullptr;
    for (auto* channel : nextChannels) {
        // do not consider the boundary channel and only consider Normal channels
        if (channel == boundaryChannel || channel->getChannelType() != arch::ChannelType::NORMAL) {
            continue;
        }

        // get the correct direction of the flow rate, where a positive flow rate means an outflow (away from the droplet center)
        double flowRate = channel->getNode0() == node ? channel->getFlowRate() : -(channel->getFlowRate());

        // only consider channels with a positive flow rate
        if (flowRate <= 0) {
            continue;
        }

        // find maximal flow rate
        if (nextChannel == nullptr || flowRate > maxFlowRate) {
            maxFlowRate = flowRate;
            nextChannel = channel;
        }
    }

    if (nextChannel == nullptr) {
        // no new channel was found => the boundary goes into a Wait state
        boundary.setState(BoundaryState::WAIT_OUTFLOW);
        return;
    }

    // check if the droplet was in a single channel, if not then the boundary channel has to be added as fully occupied channel
    if (!droplet.isInsideSingleChannel()) {
        droplet.addFullyOccupiedChannel(boundaryChannel);
    }

    // get new position (is either 0.0 or 1.0, depending on if node0 or node1 of the nextChannel is the referenceNode)
    double channelPosition = nextChannel->getNode0() == node ? 0.0 : 1.0;
    bool volumeTowardsNode0 = nextChannel->getNode0() == node;

    // set new channel, position, direction of volume, and state of the boundary
    boundary.getChannelPosition().setChannel(nextChannel);
    boundary.getChannelPosition().setPosition(channelPosition);
    boundary.setVolumeTowardsNode0(volumeTowardsNode0);
    boundary.setState(BoundaryState::NORMAL);
}

}  // namespace sim