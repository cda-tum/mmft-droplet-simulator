/**
 * @file BoundaryTailEvent.h
 */
#pragma once

#include "../../architecture/Chip.h"
#include "../Droplet.h"
#include "../DropletBoundary.h"
#include "Event.h"

namespace sim {

/**
 * @brief Class for a boundary tail event that takes place when the tail boundary reaches the end of a channel.
 */
class BoundaryTailEvent : public Event {
  private:
    Droplet& droplet;           ///< Droplet for which the event should take place.
    DropletBoundary& boundary;  ///< Boundary of the droplet that is effected by the event.
    const arch::Chip& chip;     ///< Microfluidic biochip that is simulated.

  public:
    /**
     * @brief Construct a new boundary tail event.
     * @param time Time at which the event should take place, in s elapsed since the start of the simulation.
     * @param droplet Droplet for which the event should happen.
     * @param boundary Boundary that is effected by the event.
     * @param chip Microfluidic biochip.
     */
    BoundaryTailEvent(double time, Droplet& droplet, DropletBoundary& boundary, const arch::Chip& chip);

    /**
     * @brief Perform the boundary tail event.
     */
    void performEvent() override;
};

}  // namespace sim
