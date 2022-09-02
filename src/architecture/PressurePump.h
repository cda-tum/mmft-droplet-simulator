/**
 * @file PressurePump.h
 */

#pragma once

#include "Edge.h"
#include "IPressurePump.h"
#include "Node.h"

namespace arch {

/**
 * @brief Class to specify a pressure pump, which is a component of a chip.
 */
class PressurePump : public virtual Edge, public virtual nodal::IPressurePump {
  private:
    double pressure;      ///< Pressure of pump in Pa.
    double flowRate = 0;  ///< Flow rate of pump in m^3/s.

  public:
    /**
     * @brief Constructor to create pressure pump.
     * @param[in] id Id of the pressure pump.
     * @param[in] node0 Pointer to node at one end of the pressure pump.
     * @param[in] node1 Pointer to node at other end of the pressure pump.
     * @param[in] pressure Pressure of the pump in Pa.
     */
    PressurePump(int id, Node* node0, Node* node1, double pressure);

    /**
     * @brief Set pressure of the pump.
     * @param[in] pressure Pressure of the pump in Pa.
     */
    void setPressure(double pressure);

    /**
     * @brief Set flow rate of the pump.
     * @param[in] flowRate Flow rate of pump in m^3/s.
     */
    void setFlowRate(double flowRate) override;

    /**
     * @brief Get pressure of pump.
     * @return Pressure of pump in Pa.
     */
    double getPressure() const override;

    /**
     * @brief Get flow rate of pump.
     * @return Flow rate of pump in m^3/s.
     */
    double getFlowRate() const override;

    /**
     * @brief Get resistance of pump.
     * @return Resistance of pump in Pas/L.
     */
    double getResistance() const override;
};

}  // namespace arch
