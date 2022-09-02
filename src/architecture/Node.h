/**
 * @file Node.h
 */
#pragma once

#include <string>

#include "INode.h"

namespace arch {

/**
 * @brief Class to specify a node, which is an endpoint of an edge and defines connections between components of a chip.
 */
class Node : public virtual nodal::INode {
  private:
    int const id;           ///< Id of the node.
    std::string name = "";  ///< Name of the node.
    double pressure = 0;    ///< Pressure of the node in Pa.

  public:
    /**
     * @brief Constructor of the node.
     * @param[in] id Id of the node.
     */
    Node(int id);

    /**
     * @brief Set name of the node.
     * @param[in] name Name of the node.
     */
    void setName(std::string name);

    /**
     * @brief Set pressure level at the node.
     * @param[in] pressure Pressure level at the node in Pa.
     */
    void setPressure(double pressure) override;

    /**
     * @brief Get name of the node.
     * @return Name of the node.
     */
    std::string getName() const;

    /**
     * @brief Get id of the node.
     * @return Id of node.
     */
    int getId() const override;

    /**
     * @brief Get pressure level at node.
     * @return Pressure level at node.
     */
    double getPressure() const;
};

}  // namespace arch