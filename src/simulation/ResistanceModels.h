/**
 * @file ResistanceModels.h
 */

#pragma once

#include "IResistanceModel.h"

namespace sim {

/**
 * @brief Class that defines the functionality of the 1D resistance model.
 */
class ResistanceModel0 : public sim::IResistanceModel {
  private:
    double continuousPhaseViscosity;  ///< The viscosity of the continuous phase in Pas.

  public:
    /**
     * @brief Instantiate the resistance model.
     * @param[in] continuousPhaseViscosity The viscosity of the continuous phase in Pas.
     */
    ResistanceModel0(double continuousPhaseViscosity);

    /**
     * @brief Calculate and returns the resistance of the channel itself.
     * @param[in] channel A pointer to the channel for which the resistance should be calculated.
     * @return The resistance of the channel itself in Pas/L.
     */
    double getChannelResistance(arch::Channel const* const channel) const override;

    /**
     * @brief Compute the a factor.
     * @param[in] width Width of the channel in m.
     * @param[in] height Height of the channel in m.
     * @return The a factor.
     */
    double computeFactorA(double width, double height) const;

    /**
     * @brief Retrieve the resistance caused by the specified droplet in the specified channel.
     * @param[in] channel Pointer to channel for which the droplet resistance should be calculated.
     * @param[in] droplet Pointer to droplet that causes the droplet resistance in the channel.
     * @param[in] volumeInsideChannel The volume inside the channel in m^3.
     * @return Resistance caused by the droplet in the channel in Pas/L.
     */
    double getDropletResistance(arch::Channel const* const channel, sim::Droplet* droplet, double volumeInsideChannel) const override;
};

/**
 * @brief Class that defines a test resistance model.
 */
class ResistanceModel1 : public sim::IResistanceModel {
  public:
    /**
     * @brief Instantiate the resistance model.
     */
    ResistanceModel1();

    /**
     * @brief Calculate and returns the resistance of the channel itself.
     * @param[in] channel Channel for which the resistance should be calculated.
     * @return The resistance of the channel in Pas/L.
     */
    double getChannelResistance(arch::Channel const* const channel) const override;

    /**
     * @brief Retrieve the resistance caused by the specified droplet in the specified channel.
     * @param[in] channel Pointer to channel for which the droplet resistance should be calculated.
     * @param[in] droplet Pointer to droplet that causes the droplet resistance in the channel.
     * @param[in] volumeInsideChannel The volume inside the channel in m^3.
     * @return Resistance caused by the droplet in the channel in Pas/L.
     */
    double getDropletResistance(arch::Channel const* const channel, sim::Droplet* droplet, double volumeInsideChannel) const override;
};

}  // namespace sim
