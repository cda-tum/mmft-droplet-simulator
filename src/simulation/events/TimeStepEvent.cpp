#include "TimeStepEvent.h"

namespace sim {

TimeStepEvent::TimeStepEvent(double time) : Event(time, 2) {}

void TimeStepEvent::performEvent() {
    // this event does nothing and only ensures a minimal time step, so a new flow state is computed again
    return;
}

}  // namespace sim