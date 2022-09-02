/**
 * @file IPressurePump.h
 */
#pragma once

#include "IEdge.h"

namespace nodal {

/**
 * @brief Interface of a pressure pump required for the nodal analysis. A pressure pump is a component of a chip.
 */
class IPressurePump : public virtual IEdge {
  public:
    /**
     * @brief Virtual constructor of a pressure pump.
     */
    virtual ~IPressurePump() {}

    /**
     * @brief Function that returns the pressure of a pressure pump.
     * @returns Pressure of the pump in Pa.
     */
    virtual double getPressure() const = 0;

    /**
     * @brief Function that sets the flow rate of a flow rate pump.
     * @param[in] flowRate Flow rate of the pressure pump in m^3/s.
     */
    virtual void setFlowRate(double flowRate) = 0;
};

}  // namespace nodal
