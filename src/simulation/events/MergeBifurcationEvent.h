#pragma once

#include "Droplet.h"
#include "DropletBoundary.h"
#include "Event.h"
#include "Simulation.h"

namespace sim {

/**
 * @brief Class that specifies a merge event that takes place at a bifurcation.
 */
class MergeBifurcationEvent : public Event {
  private:
    Droplet& droplet0;           ///< First droplet that will be merged during that event (is the droplet that flows into the droplet at the bifurcation).
    Droplet& droplet1;           ///< Second droplet that will be merged (is the droplet at the bifurcation).
    DropletBoundary& boundary0;  ///< The boundary of droplet0 (will be "removed" from the merged droplet).
    Simulation& simulation;      ///< Simulation class

  public:
    /**
     * @brief Constructs a new Merge Bifurcation Event object
     *
     * @param time At which the event takes place (elapsed since the start of the simulation) in s.
     * @param droplet0 First droplet that will be merged during that event (is the droplet that flows into the droplet at the bifurcation).
     * @param boundary0 The boundary of droplet0 (will be "removed" from the merged droplet).
     * @param droplet1 Second droplet that will be merged (is the droplet at the bifurcation).
     * @param simulation Simulation class.
     */
    MergeBifurcationEvent(double time, Droplet& droplet0, DropletBoundary& boundary0, Droplet& droplet1, Simulation& simulation);

    /**
     * @brief Conducts the merge at bifurcation event.
     */
    void performEvent() override;
};

}  // namespace sim