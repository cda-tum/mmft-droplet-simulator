#include "DropletBoundary.h"

#include "Channel.h"
#include "ChannelPosition.h"
#include "Chip.h"

namespace sim {

DropletBoundary::DropletBoundary(arch::Channel* channel, double position, bool volumeTowardsNode0, BoundaryState state) : channelPosition(channel, position), volumeTowardsNode0(volumeTowardsNode0), state(state) {}

arch::ChannelPosition& DropletBoundary::getChannelPosition() {
    return channelPosition;
}

double DropletBoundary::getFlowRate() const {
    return flowRate;
}

bool DropletBoundary::isVolumeTowardsNode0() const {
    return volumeTowardsNode0;
}

BoundaryState DropletBoundary::getState() const {
    return state;
}

void DropletBoundary::setFlowRate(double flowRate) {
    this->flowRate = flowRate;
}

void DropletBoundary::setVolumeTowardsNode0(bool volumeTowardsNode0) {
    this->volumeTowardsNode0 = volumeTowardsNode0;
}

void DropletBoundary::setState(BoundaryState state) {
    this->state = state;
}

arch::Node* DropletBoundary::getReferenceNode() {
    if (volumeTowardsNode0) {
        return channelPosition.getChannel()->getNode0();
    } else {
        return channelPosition.getChannel()->getNode1();
    }
}

arch::Node* DropletBoundary::getOppositeReferenceNode() {
    if (volumeTowardsNode0) {
        return channelPosition.getChannel()->getNode1();
    } else {
        return channelPosition.getChannel()->getNode0();
    }
}

double DropletBoundary::getRemainingVolume() {
    double volume = 0;

    if (flowRate < 0) {
        // boundary moves towards the droplet center
        if (volumeTowardsNode0) {
            volume = channelPosition.getVolume0();
        } else {
            volume = channelPosition.getVolume1();
        }
    } else if (flowRate > 0) {
        // boundary moves away from the droplet center
        if (volumeTowardsNode0) {
            volume = channelPosition.getVolume1();
        } else {
            volume = channelPosition.getVolume0();
        }
    }

    return volume;
};

double DropletBoundary::getVolume() {
    if (volumeTowardsNode0) {
        return channelPosition.getVolume0();
    } else {
        return channelPosition.getVolume1();
    }
}

double DropletBoundary::getChannelFlowRate() {
    double flowRate = channelPosition.getChannel()->getFlowRate();
    if (volumeTowardsNode0) {
        return flowRate;
    } else {
        return -flowRate;
    }
}

double DropletBoundary::getTime() {
    double time = 0;

    if (flowRate < 0) {
        time = getRemainingVolume() / -flowRate;
    } else if (flowRate > 0) {
        time = getRemainingVolume() / flowRate;
    }

    return time;
}

void DropletBoundary::moveBoundary(double timeStep) {
    // check in which direction the volume goes
    if (volumeTowardsNode0) {
        // positive flow rate indicates an outflow (movement towards node1) and, thus, the position inside the channel must increase
        // negative flow rate indicates an inflow (movement towards node0) and, thus, the position inside the channel must decrease
        channelPosition.addToPosition(flowRate * timeStep);
    } else {
        // positive flow rate indicates an outflow (movement towards node0) and, thus, the position inside the channel must decrease
        // negative flow rate indicates an inflow (movement towards node1) and, thus, the position inside the channel must increase
        channelPosition.addToPosition(-flowRate * timeStep);
    }
}

void DropletBoundary::updateWaitState(arch::Chip const& chip) {
    if (state == BoundaryState::WAIT_INFLOW) {
        // in this scenario the boundary is in a Wait state while an inflow occurred (movement to the droplet center)
        // in this implementation this does not happen, because the boundary gets immediately deleted when it would reach this state (was not the case in a previous concept)
        // however, it is here for consistency and possible future implementations

        // get oriented channel flow rate
        double channelFlowRate = getChannelFlowRate();

        // check if the flow rate is not an inflow anymore (>0)
        // this indicates that the flow rate has changed since the last state and that the state should be Normal again
        if (channelFlowRate > 0) {
            state = BoundaryState::NORMAL;
        }
    } else if (state == BoundaryState::WAIT_OUTFLOW) {
        // in this scenario the boundary is in a Wait state while an outflow occurred (movement away from the droplet center)
        // this scenario happens at the "end" of a channel when a boundary wants to switch to another channel
        // it indicates that the boundary could not move any further, because no channel was available that matches the criteria when a boundary switches channels (e.g., only a bypass channel was available or no flow rate of the other channels had an outflow)
        // hence, it must be checked if the actual flow rate inside the channel has changed, or if there are now channels available that matches the criteria

        // get oriented channel flow rate
        double channelFlowRate = getChannelFlowRate();

        // check if the flow rate is not an outflow anymore (<0)
        // this indicates that the flow rate has changed since the last state and that the state should be Normal again (the boundary would move in the other direction again)
        if (channelFlowRate < 0) {
            state = BoundaryState::NORMAL;
            return;
        }

        // if the flow rate did not change, then check for valid channels
        auto boundaryChannel = channelPosition.getChannel();
        int nodeId = isVolumeTowardsNode0() ? boundaryChannel->getNode1()->getId() : boundaryChannel->getNode0()->getId();
        for (auto& channel : chip.getChannelsAtNode(nodeId)) {
            // do not consider boundary channel or channel that is not a Normal one
            if (channel == boundaryChannel || channel->getChannelType() != arch::ChannelType::NORMAL) {
                continue;
            }

            // check if a channel exists with an outflow (flow rate goes away from the node)
            double flowRate = nodeId == channel->getNode0()->getId() ? channel->getFlowRate() : -channel->getFlowRate();
            if (flowRate > 0) {
                state = BoundaryState::NORMAL;
                return;
            }
        }
    }
}

bool DropletBoundary::isInWaitState() {
    return state == BoundaryState::WAIT_INFLOW || state == BoundaryState::WAIT_OUTFLOW;
}

}  // namespace sim