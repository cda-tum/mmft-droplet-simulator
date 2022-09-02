/**
 * @file SimulatorImpl.h
 */
#pragma once

#include "Chip.h"
#include "Simulation.h"

namespace droplet {

/**
 * @brief Class that contains the implementation of the simulator.
 */
class SimulatorImpl {
  public:
    arch::Chip chip;             ///< Chip to be simulated.
    sim::Simulation simulation;  ///< Simulation itself.

    /**
     * @brief Implementation of the simulator.
     */
    SimulatorImpl();
};

}  // namespace droplet