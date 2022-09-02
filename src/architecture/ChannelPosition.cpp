#include "ChannelPosition.h"

#include "Channel.h"

namespace arch {

ChannelPosition::ChannelPosition(Channel* channel, double position) : channel(channel), position(position) {}

void ChannelPosition::setChannel(Channel* const channel) {
    this->channel = channel;
}

void ChannelPosition::setPosition(double position) {
    // ensure that position stays in range (e.g., due to rounding errors)
    if (position < 0.0) {
        this->position = 0.0;
    } else if (position > 1.0) {
        this->position = 1.0;
    } else {
        this->position = position;
    }
}

void ChannelPosition::addToPosition(double volumeShift) {
    double newPosition = position + volumeShift / channel->getVolume();
    setPosition(newPosition);
}

Channel* ChannelPosition::getChannel() const {
    return channel;
}

double ChannelPosition::getPosition() const {
    return position;
}

double ChannelPosition::getAbsolutePosition() const {
    return position * channel->getLength();
}

double ChannelPosition::getVolume0() const {
    return position * channel->getVolume();
}

double ChannelPosition::getVolume1() const {
    return (1.0 - position) * channel->getVolume();
}

}  // namespace arch