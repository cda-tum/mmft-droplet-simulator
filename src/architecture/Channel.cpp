#include "Channel.h"

#include "Edge.h"
#include "Node.h"

namespace arch {

Channel::Channel(int id, Node* node0, Node* node1, double height, double width, double length, ChannelType type) : Edge(id, node0, node1), height(height), width(width), length(length), type(type) {}

Channel::Channel(int id, Node* node0, Node* node1, double resistance, ChannelType type) : Edge(id, node0, node1), channelResistance(resistance), type(type) {}

void Channel::setDimensions(double width, double height, double length) {
    this->height = height;
    this->width = width;
    this->length = length;
}

void Channel::setHeight(double height) {
    this->height = height;
}

void Channel::setWidth(double width) {
    this->width = width;
}

void Channel::setLength(double length) {
    this->length = length;
}

void Channel::setChannelResistance(double channelResistance) {
    this->channelResistance = channelResistance;
}

void Channel::setDropletResistance(double dropletResistance) {
    this->dropletResistance = dropletResistance;
}

void Channel::setChannelType(ChannelType type) {
    this->type = type;
}

void Channel::addDropletResistance(double dropletResistance) {
    this->dropletResistance += dropletResistance;
}

double Channel::getHeight() const {
    return height;
}

double Channel::getWidth() const {
    return width;
}

double Channel::getLength() const {
    return length;
}

double Channel::getArea() const {
    return width * height;
}

double Channel::getVolume() const {
    return width * height * length;
}

double Channel::getPressure() const {
    return node0->getPressure() - node1->getPressure();
}

double Channel::getFlowRate() const {
    return getPressure() / getResistance();
}

double Channel::getResistance() const {
    return channelResistance + dropletResistance;
}

double Channel::getChannelResistance() const {
    return channelResistance;
}

double Channel::getDropletResistance() const {
    return dropletResistance;
}

ChannelType Channel::getChannelType() const {
    return type;
}

}  // namespace arch