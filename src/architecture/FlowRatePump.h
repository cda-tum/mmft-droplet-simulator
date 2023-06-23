/**
 * @file FlowRatePump.h
 */

#pragma once

#include "Edge.h"
#include "../nodalAnalysis/IFlowRatePump.h"
#include "Node.h"

namespace arch {

/**
 * @brief Class to specify a flow rate pump, which is a component of a chip.
 */
class FlowRatePump : public virtual Edge, public virtual nodal::IFlowRatePump {
  private:
    double flowRate;  ///< Volumetric flow rate of the pump in m^3/s.

  public:
    /**
     * @brief Constructor to create a flow rate pump.
     * @param[in] id Id of the flow rate pump.
     * @param[in] node0 Pointer to node at one end of the flow rate pump.
     * @param[in] node1 Pointer to node at other end of the flow rate pump.
     * @param[in] flowRate Volumetric flow rate of the pump in m^3/s.
     */
    FlowRatePump(int id, Node* node0, Node* node1, double flowRate);

    /**
     * @brief Set volumetric flow rate of the pump.
     * @param[in] flowRate New volumetric flow rate to set in m^3/s.
     */
    void setFlowRate(double flowRate);

    /**
     * @brief Get pressure of the pump.
     * @return Pressure of flow rate pump in Pa.
     */
    double getPressure() const override;

    /**
     * @brief Get volumetric flow rate of pump.
     * @return Volumetric flow rate of pump in m^3/s.
     */
    double getFlowRate() const override;

    /**
     * @brief Get resistance over flow rate pump.
     * @return Resistance over flow rate pump in Pas/L.
     */
    double getResistance() const override;
};

}  // namespace arch
