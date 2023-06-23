/**
 * @file IResistanceModel.h
 */

#pragma once

#include "../architecture/Channel.h"

namespace sim {

class Droplet;

/**
 * @brief Class that specifies the interface of an resistance model. The simulation flow calculations are conducted on the basis of a resistance model.
 */
class IResistanceModel {
  public:
    /**
     * @brief Virtual constructor of a resistance model.
     */
    virtual ~IResistanceModel() {}

    /**
     * @brief Retrieve resistance of the channel itself.
     * @param[in] channel Pointer to channel for which the resistance should be calculated.
     * @return The resistance in the channel itself in Pas/L.
     */
    virtual double getChannelResistance(arch::Channel const* const channel) const = 0;

    /**
     * @brief Retrieve resistance caused by the droplet within the channel.
     * @param[in] channel Pointer to channel in which the droplet currently is.
     * @param[in] droplet Pointer to droplet which causes resistance.
     * @param[in] volumeInsideChannel Volume inside the channel in m^3.
     * @return Resistance caused by the droplet in this channel in Pas/L.
     */
    virtual double getDropletResistance(arch::Channel const* const channel, Droplet* droplet, double volumeInsideChannel) const = 0;
};

}  // namespace sim
