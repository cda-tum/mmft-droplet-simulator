/**
 * @file IFlowRatePump.h
 */

#pragma once

#include "IEdge.h"

namespace nodal {

/**
 * @brief Interface of a flow rate pump that specifies the functions required for the nodal analysis. A flow rate pump is a component of a chip.
 */
class IFlowRatePump : public virtual IEdge {
  public:
    /**
     * @brief Constructor to create an IFlowRatePump.
     */
    virtual ~IFlowRatePump() {}

    /**
     * @brief Function that returns the volumetric flow rate of the flow rate pump.
     * @return Volumetric flow rate of the pump in m^3/s.
     */
    virtual double getFlowRate() const = 0;
};

}  // namespace nodal