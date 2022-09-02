/**
 * @file Injection.h
 */

#pragma once

#include <memory>
#include <string>

#include "ChannelPosition.h"
#include "Droplet.h"

namespace sim {

/**
 * @brief Class that contains all paramaters necessary to conduct an injection.
 */
class Injection {
  private:
    const int id;                             ///< Unique identifier of an injection.
    std::string name = "";                    ///< Name of the injection.
    double injectionTime;                     ///< Time at which the injection should take place in s elapsed since the start of the simulation.
    Droplet* droplet;                         ///< Pointer to droplet to be injected.
    arch::ChannelPosition injectionPosition;  ///< Position at which the droplet should be injected.

  public:
    /**
     * @brief Create an injection.
     * @param[in] id Unique identifier of an injection.
     * @param[in] droplet Pointer to the droplet to be injected.
     * @param[in] injectionTime Time at which the droplet should be injected in s elapsed since the start of the simulation in s.
     * @param[in] channel Channel in which the droplet should be injected. The channel must be able to fully contain the droplet.
     * @param[in] injectionPosition Relative position (between 0.0 and 1.0) of the middle of the droplet in channel. Head and tail position must be in same channel.
     */
    Injection(int id, Droplet* droplet, double injectionTime, arch::Channel* channel, double injectionPosition);

    /**
     * @brief Set name of injection.
     * @param[in] name Name of injection.
     */
    void setName(std::string name);

    /**
     * @brief Retrieve unique identifier of injection.
     * @return Unique identifier of injection.
     */
    int getId() const;

    /**
     * @brief Retireve name of injection.
     * @return Name of injection.
     */
    std::string getName() const;

    /**
     * @brief Retrieve time at which the droplet should be injected.
     * @return Time in s elapsed since the start of the simulation at which the injection of the droplet should take place in s.
     */
    double getInjectionTime() const;

    /**
     * @brief Retrieve position at which the injection should take place.
     * @return Position at which the injection should take place. An injection can only take place in channels that are able to fully contain droplet.
     */
    const arch::ChannelPosition& getInjectionPosition() const;

    /**
     * @brief Retrieve pointer to the droplet that should be injected.
     * @return Droplet to be injected.
     */
    Droplet* getDroplet() const;
};

}  // namespace sim