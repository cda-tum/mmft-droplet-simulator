#pragma once

#include <iostream>
#include <iterator>

#include "Droplet.h"
#include "DropletBoundary.h"
#include "Event.h"
#include "Simulation.h"

namespace sim {

/**
 * @brief Class that specifies a merge event that takes place within a channel.
 */
class MergeChannelEvent : public Event {
  private:
    Droplet& droplet0;           ///< First droplet that will be merged during that event.
    Droplet& droplet1;           ///< Second droplet that will be merged during that event.
    DropletBoundary& boundary0;  ///< The boundary of droplet0 (will be "removed" from the merged droplet).
    DropletBoundary& boundary1;  ///< The boundary of droplet1 (will be "removed" from the merged droplet).
    Simulation& simulation;      ///< Simulation class

  public:
    /**
     * @brief Construct a new Merge Channel Event object
     * @param time Time at which the event takes place in s elapsed since the start of the simulation
     * @param droplet0 First droplet that will be merged during that event.
     * @param boundary0 The boundary of droplet0 (will be "removed" from the merged droplet).
     * @param droplet1 Second droplet that will be merged during that event.
     * @param boundary1 The boundary of droplet1 (will be "removed" from the merged droplet).
     * @param simulation Simulation class.
     */
    MergeChannelEvent(double time, Droplet& droplet0, DropletBoundary& boundary0, Droplet& droplet1, DropletBoundary& boundary1, Simulation& simulation);

    /**
     * @brief  Perform the merge that happens within a channel sequence.
     */
    void performEvent() override;
};

}  // namespace sim