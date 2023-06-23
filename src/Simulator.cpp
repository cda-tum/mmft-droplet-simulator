#include "../droplet-simulator/Simulator.h"

#include <iostream>
#include <memory>

#include "architecture/Channel.h"
#include "architecture/Chip.h"
#include "../droplet-simulator/Results.h"
#include "SimulatorImpl.h"

namespace droplet {

Simulator::Simulator() : impl(std::make_unique<SimulatorImpl>()) {}

Simulator::~Simulator() = default;

int Simulator::addChannel(int node0Id, int node1Id, double height, double width, double length) {
    return impl->chip.addChannel(node0Id, node1Id, height, width, length, arch::ChannelType::NORMAL);
}

int Simulator::addBypassChannel(int node0Id, int node1Id, double height, double width, double length) {
    return impl->chip.addChannel(node0Id, node1Id, height, width, length, arch::ChannelType::BYPASS);
}

int Simulator::addFlowRatePump(int node0Id, int node1Id, double flowRate) {
    return impl->chip.addFlowRatePump(node0Id, node1Id, flowRate);
}

int Simulator::addPressurePump(int node0Id, int node1Id, double pressure) {
    return impl->chip.addPressurePump(node0Id, node1Id, pressure);
}

void Simulator::addSink(int nodeId) {
    impl->chip.addSink(nodeId);
}

void Simulator::addGround(int nodeId) {
    impl->chip.addGround(nodeId);
}

bool Simulator::checkChipValidity() {
    return impl->chip.isNetworkValid();
}

int Simulator::addFluid(double viscosity, double density, double concentration) {
    return impl->simulation.addFluid(viscosity, density, concentration)->getId();
}

void Simulator::setContinuousPhase(int fluidId) {
    impl->simulation.setContinuousPhase(fluidId);
}

void Simulator::setMaximalAdaptiveTimeStep(double timeStep) {
    impl->simulation.setMaximalAdaptiveTimeStep(timeStep);
}

int Simulator::addDroplet(int fluidId, double volume, double injectionTime, int channelId, double relInjectionPosition) {
    int dropletId = impl->simulation.addDroplet(fluidId, volume)->getId();
    impl->simulation.addInjection(dropletId, injectionTime, channelId, relInjectionPosition);
    return dropletId;
}

SimulationResult Simulator::simulate() {
    return impl->simulation.simulate();
}

SimulatorImpl::SimulatorImpl() {
    simulation.setChip(&chip);
}

}  // namespace droplet
