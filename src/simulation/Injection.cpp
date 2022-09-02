#include "Injection.h"

#include <memory>
#include <utility>

#include "ChannelPosition.h"

namespace sim {

Injection::Injection(int id, Droplet* droplet, double injectionTime, arch::Channel* channel, double injectionPosition) : id(id), droplet(droplet), injectionTime(injectionTime), injectionPosition(arch::ChannelPosition(channel, injectionPosition)) {}

void Injection::setName(std::string name) {
    name = std::move(name);
}

int Injection::getId() const {
    return id;
}

std::string Injection::getName() const {
    return name;
}

double Injection::getInjectionTime() const {
    return injectionTime;
}

const arch::ChannelPosition& Injection::getInjectionPosition() const {
    return injectionPosition;
}

Droplet* Injection::getDroplet() const {
    return droplet;
}

}  // namespace sim