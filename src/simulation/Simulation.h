/**
 * @file Simulation.h
 */

#pragma once

#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../architecture/Chip.h"
#include "Droplet.h"
#include "events/Event.h"
#include "Fluid.h"
#include "IResistanceModel.h"
#include "Injection.h"
#include "../droplet-simulator/Results.h"

namespace sim {

/**
 * @brief Enum to define the available resistance models.
 */
enum class ResistanceModel {
    ONE_D_MODEL,  ///< 1D Resistance Model.
    TEST_MODEL    ///< Test Resistance Model.
};

/**
 * @brief Class that conducts the simulation and owns all parameters necessary for it.
 */
class Simulation {
  private:
    arch::Chip* chip;                                                    ///< Chip for which the simulation should be conducted.
    std::unordered_map<int, std::unique_ptr<Fluid>> fluids;              ///< Fluids specified for the simulation.
    std::unordered_map<int, std::unique_ptr<Droplet>> droplets;          ///< Droplets which are simulated.
    std::unordered_map<int, std::unique_ptr<Injection>> injections;      ///< Injections of droplets that should take place during the simulation.
    std::unique_ptr<IResistanceModel> resistanceModel;                   ///< The resistance model used for the simulation.
    ResistanceModel resistanceModelName = ResistanceModel::ONE_D_MODEL;  ///< Which resistance model should be used for the calculations of this simulation.
    Fluid* continuousPhase = nullptr;                                    ///< Fluid of the continuous phase.
    double const slipFactor = 1.28;                                      ///< Slip factor of droplets.
    double maximalAdaptiveTimeStep = 0;                                  ///< Maximal adaptive time step that is applied when droplets change the channel.
    double currTime = 0;                                                 ///< The time elapsed since the start of the simulation in s.
    double iState = 0;                                                   ///< The current index of the next state.
    bool dropletsAtBifurcation = false;                                  ///< If one or more droplets are currently at a bifurcation. Triggers the usage of the maximal adaptive time step.
    bool enableMerging = true;                                           ///< If the droplet merging simulation should be performed.
    unsigned maxIterations = 1000000;                                    ///< Maximum iterations performed before computations are aborted.

    /**
     * @brief Initializes the resistance model and the channel resistances of the empty channels.
     */
    void initialize();

    /**
     * @brief Update the droplet resistances of the channels based on the current positions of the droplets.
     */
    void updateDropletResistances();

    /**
     * @brief Compute all possible next events.
     */
    std::vector<std::unique_ptr<Event>> computeEvents();

    /**
     * @brief Moves all droplets according to the given time step.
     * @param[in] timeStep to which the droplets should be moved to. TODO
     */
    void moveDroplets(double timeStep);

    /**
     * @brief Store simulation parameters to the result.
     * @param[in, out] result Reference to the simulation result in which all current parameters of the simulation should be stored.
     */
    void storeSimulationParameters(droplet::SimulationResult& result);

    /**
     * @brief Store all simulation results to the result.
     * @param[in, out] result Reference to the simulation result in which all current parameters of the simulation should be stored.
     */
    void storeSimulationResults(droplet::SimulationResult& result);

  public:
    /**
     * @brief Creates simulation.
     */
    Simulation();

    /**
     * @brief Set the chip for which the simulation should be conducted.
     * @param[in] chip Chip on which the simulation will be conducted.
     */
    void setChip(arch::Chip* chip);

    /**
     * @brief Get the chip.
     * @return Chip or nullptr if no chip is specified.
     */
    arch::Chip* getChip();

    /**
     * @brief Define which fluid should act as continuous phase, i.e., as carrier fluid for the droplets.
     * @param[in] fluidId Unique identifier of the fluid the continuous phase consists of.
     */
    void setContinuousPhase(int fluidId);

    /**
     * @brief Get the continuous phase.
     * @return Fluid if the continuous phase or nullptr if no continuous phase is specified.
     */
    Fluid* getContinuousPhase();

    /**
     * @brief Define which resistance model should be used for the channel and droplet resistance calculations.
     * @param[in] modelName Name of the resistance model to be used.
     */
    void setResistanceModel(ResistanceModel modelName);

    /**
     * @brief Define the maximal adaptive time step of the simulation.
     This time step is applied when a droplet changes channels in order to increase the simulation accuracy.
     A value of 0 disables this behavior (default is 0).
     * @param[in] timeStep Maximal time step that the simulation can do when changing a channel.
     */
    void setMaximalAdaptiveTimeStep(double timeStep);

    /**
     * @brief Create fluid.
     * @param[in] viscosity Viscosity of the fluid in Pas.
     * @param[in] density Density of the fluid in kg/m^3.
     * @param[in] concentration Concentration of the fluid in % (between 0.0 and 1.0).
     * @return Pointer to created fluid.
     */
    Fluid* addFluid(double viscosity, double density, double concentration);

    /**
     * @brief Get fluid.
     * @param[in] fluidId Id of the fluid
     * @return Pointer to fluid with the corresponding id
     */
    Fluid* getFluid(int fluidId);

    /**
     * @brief Create droplet.
     * @param[in] fluidId Unique identifier of the fluid the droplet consists of.
     * @param[in] volume Volume of the fluid in m^3.
     * @return Pointer to created droplet.
     */
    Droplet* addDroplet(int fluidId, double volume);

    /**
     * @brief Get droplet
     * @param dropletId Id of the droplet
     * @return Pointer to droplet with the corresponding id
     */
    Droplet* getDroplet(int dropletId);

    /**
     * @brief Gets droplet that is present at the corresponding node (i.e., the droplet spans over this node).
     * @param nodeId The id of the node
     * @return Pointer to droplet or nullptr if no droplet was found
     */
    Droplet* getDropletAtNode(int nodeId);

    /**
     * @brief Create injection.
     * @param[in] dropletId Id of the droplet that should be injected.
     * @param[in] injectionTime Time at which the droplet should be injected in s.
     * @param[in] channelId Id of the channel, where droplet should be injected.
     * @param[in] injectionPosition Position inside the channel at which the droplet should be injected (relative to the channel length between 0.0 and 1.0).
     * @return Pointer to created injection.
     */
    Injection* addInjection(int dropletId, double injectionTime, int channelId, double injectionPosition);

    /**
     * @brief Get injection
     * @param injectionId The id of the injection
     * @return Pointer to injection with the corresponding id.
     */
    Injection* getInjection(int injectionId);

    /**
     * @brief Creates a new fluid out of two existing fluids.
     * @param fluid0Id Id of the first fluid.
     * @param volume0 The volume of the first fluid.
     * @param fluid1Id Id of the second fluid.
     * @param volume1 The volume of the second fluid.
     * @return Pointer to new fluid.
     */
    Fluid* mixFluids(int fluid0Id, double volume0, int fluid1Id, double volume1);

    /**
     * @brief Creates a new droplet from two existing droplets.
     Please note that this only creates a new droplet inside the simulation, but the actual boundaries have to be set separately, which is usually done inside the corresponding merge events.
     * @param droplet0Id Id of the first droplet.
     * @param droplet1Id Id of the second droplet.
     * @return Pointer to new droplet.
     */
    Droplet* mergeDroplets(int droplet0Id, int droplet1Id);

    /**
     * @brief Conduct the simulation.
     * @return The result of the simulation containing all intermediate simulation steps and calculated parameters.
     */
    droplet::SimulationResult simulate();
};

}  // namespace sim