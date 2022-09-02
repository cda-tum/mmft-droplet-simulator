/**
 * @file InjectionEvent.h
 */

#pragma once

#include "Event.h"
#include "Injection.h"

namespace sim {

/**
 * @brief Class for an injection event that takes place when a droplet is injected into the network.
 */
class InjectionEvent : public Event {
  private:
    Injection& injection;  ///< Specifies if the injection event.

  public:
    /**
     * @brief Definies an injection event to take place at a certain time.
     * @param[in] time The time at which the event should take place in s elapsed since the start of the simulation.
     * @param[in,out] injection A class containing all details necessary to conduct an injection event.
     */
    InjectionEvent(double time, Injection& injection);

    /**
     * @brief Conducts the injection event.
     */
    void performEvent() override;
};

}  // namespace sim