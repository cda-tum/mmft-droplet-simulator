/**
 * @file Simulator.h
 */

#pragma once
#include <emscripten/bind.h>

#include <memory>

#include "Results.h"

namespace droplet {

/**
 * @brief Implementation of the simulator.
 */
class SimulatorImpl;

/**
 * @brief Public interface of the simulator.
 */
class Simulator {
  private:
    std::unique_ptr<SimulatorImpl> impl;

  public:
    Simulator();
    ~Simulator();

    /**
     * @brief Creates and adds a normal channel to the simulator.
     * @param[in] node0Id Id of the node at one end of the channel.
     * @param[in] node1Id If of the node at other end of the channel
     * @param[in] height Height of the channel in m.
     * @param[in] width Width of the channel in m.
     * @param[in] length Length of the channel in m.
     * @return Id of the channel.
     */
    int addChannel(int node0Id, int node1Id, double height, double width, double length);

    /**
     * @brief Creates and adds bypass channel to the simulator.
     * @param[in] node0Id Id of the node at one end of the channel.
     * @param[in] node1Id Id of the node at the other end of the channel.
     * @param[in] height Height of the channel in m.
     * @param[in] width Width of the channel in m.
     * @param[in] length Length of the channel in m.
     * @return Id of the channel.
     */
    int addBypassChannel(int node0Id, int node1Id, double height, double width, double length);

    /**
     * @brief Creates and adds flow rate pump to the simulator.
     * @param[in] node0Id Id of the node at one end of the flow rate pump.
     * @param[in] node1Id Id of the node at the other end of the flow rate pump.
     * @param[in] flowRate Flow rate of the pump in m^3/s.
     * @return Id of the flow rate pump.
     */
    int addFlowRatePump(int node0Id, int node1Id, double flowRate);

    /**
     * @brief Creates and adds pressure pump to the simulator.
     * @param[in] node0Id Id of the node at one end of the pressure rate pump.
     * @param[in] node1Id Id of the node at the other end of the pressure rate pump.
     * @param[in] pressure Pressure of the pump in Pa.
     * @return Id of the pressure pump.
     */
    int addPressurePump(int node0Id, int node1Id, double pressure);

    /**
     * @brief Specifies a node as sink.
     * @param[in] nodeId
     */
    void addSink(int nodeId);

    /**
     * @brief Adds or sets a node as the ground node, i.e., this node has a pressure value of 0 and acts as a reference node for all other nodes.
     * @param[in] nodeId
     */
    void addGround(int nodeId);

    /**
     * @brief Checks validity of a chip i.e. if network is one graph and all nodes and channels are connected to ground.
     * @return If chip is valid.
     */
    bool checkChipValidity();

    /**
     * @brief Add fluid to the simulator.
     * @param[in] viscosity Viscosity of the fluid in Pa s.
     * @param[in] density Density of the fluid in kg/m^3.
     * @param[in] concentration Concentration of the fluid in % (between 0.0 and 1.0).
     * @return Id of the fluid.
     */
    int addFluid(double viscosity, double density, double concentration = 0.0);

    /**
     * @brief Specifies which fluid is the continuous phase.
     * @param[in] fluidId
     */
    void setContinuousPhase(int fluidId);

    /**
     * @brief Define the maximal adaptive time step of the simulation.
    This time step is applied when a droplet changes channels in order to increase the simulation accuracy.
    A value of 0 disables this behavior (default is 0).
     * @param[in] timeStep in s.
     */
    void setMaximalAdaptiveTimeStep(double timeStep);

    /**
     * @brief Creates and adds a droplet to the simulation.
     * @param[in] fluidId Id of the fluid the droplet consists of.
     * @param[in] volume Volume of the droplet in m^3.
     * @param[in] injectionTime Simulation time at which the droplet should be injected.
     * @param[in] channelId Id of the channel in which the droplet is injected.
     * @param[in] relInjectionPosition Relative injection position (between 0.0 and 1.0) within the injection channel.
     * @return Id of the droplet.
     */
    int addDroplet(int fluidId, double volume, double injectionTime, int channelId, double relInjectionPosition);

    /**
     * @brief Conduct the simulation.
     * @return The simulation result.
     */
    SimulationResult simulate();
};

}  // namespace droplet


EMSCRIPTEN_BINDINGS(my_class) {
  emscripten::class_<droplet::Simulator>("Simulator")
    .constructor<>()
    .function("addChannel", &droplet::Simulator::addChannel)
    .function("addBypassChannel", &droplet::Simulator::addBypassChannel)
    .function("addFlowRatePump", &droplet::Simulator::addFlowRatePump)
    .function("addPressurePump", &droplet::Simulator::addPressurePump)
    .function("addSink", &droplet::Simulator::addSink)
    .function("addGround", &droplet::Simulator::addGround)
    .function("checkChipValidity", &droplet::Simulator::checkChipValidity)
    .function("addFluid", &droplet::Simulator::addFluid)
    .function("setContinuousPhase", &droplet::Simulator::setContinuousPhase)
    .function("setMaximalAdaptiveTimeStep", &droplet::Simulator::setMaximalAdaptiveTimeStep)
    .function("addDroplet", &droplet::Simulator::addDroplet)
    .function("simulate", &droplet::Simulator::simulate)
    ;

    emscripten::class_<droplet::SimulationResult>("SimulationResult")
    .constructor<>()
    .property("continuousPhaseId", &droplet::SimulationResult::continuousPhaseId)
    .property("maximalAdaptiveTimeStep", &droplet::SimulationResult::maximalAdaptiveTimeStep)
    .property("resistanceModel", &droplet::SimulationResult::resistanceModel)
    .function("toJson", &droplet::SimulationResult::toJson)
    ;
}
