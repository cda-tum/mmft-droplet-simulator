/**
 * @file Event.h
 */
#pragma once

namespace sim {

/**
 * @brief
 * Interface for all events.
 */
class Event {
  protected:
    double time;   ///< Time at which the event should take place, in s elapsed since the start of the simulation.
    int priority;  ///< Priority of the event.

    /**
     * @brief Specifies an event to take place.
     * @param[in] time The time at which the event should take place, in s elapsed since the start of the simulation.
     * @param[in] priority Priority of an event, which is important when two events occur at the same time (the lower the value the higher the priority).
     */
    Event(double time, int priority) : time(time), priority(priority) {}

  public:
    /**
     * @brief Virtual constructor of an event to take place.
     */
    virtual ~Event() {}

    /**
     * @brief Function to get the time at which an event should take place.
     * @return Time in s (elapsed since the start of the simulation).
     */
    double getTime() const { return time; }

    /**
     * @brief Get the priority of the event
     * @return Priority value
     */
    double getPriority() const { return priority; }

    /**
     * @brief Function that is called at the time of the event to perform the event.
     */
    virtual void performEvent() = 0;
};

}  // namespace sim