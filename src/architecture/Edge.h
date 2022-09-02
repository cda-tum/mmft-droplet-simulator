/**
 * @file Edge.h
 */

#pragma once

#include <string>

#include "IEdge.h"
#include "Node.h"

namespace arch {

/**
 * @brief Abstract class to specify an edge, which is a connection between two nodes. Multiple components of a chip are an edge.
 */
class Edge : public virtual nodal::IEdge {
  protected:
    int const id;           ///< Id of the edge.
    std::string name = "";  ///< Name of the edge.
    Node* node0;            ///< Node at one end of the edge.
    Node* node1;            ///< Node at other end of the edge.

  public:
    /**
     * @brief Constructor of the edge.
     * @param[in] id Id of the edge.
     * @param[in] node0 Node at one end of the edge.
     * @param[in] node1 Node at other end of the edge.
     */
    Edge(int id, Node* node0, Node* node1);

    /**
     * @brief Set the name of an edge.
     * @param[in] name Name to be set.
     */
    void setName(std::string name);

    /**
     * @brief Get id of the edge.
     * @return Id of the edge.
     */
    int getId() const;

    /**
     * @brief Get name of the edge.
     * @return Name of the edge.
     */
    std::string getName() const;

    /**
     * @brief Get pressure over the edge.
     * @return Pressure over the edge in Pa.
     */
    virtual double getPressure() const = 0;

    /**
     * @brief Get flow rate in the edge.
     * @return Flow rate of the edge in m^3/s.
     */
    virtual double getFlowRate() const = 0;

    /**
     * @brief Get overall resistance over an edge.
     * @return Resistance over an edge in Pas/m^3.
     */
    virtual double getResistance() const = 0;

    /**
     * @brief Get pointer to node 0 (node at one end of the edge).
     * @return Pointer to node 0 (node at one end of the edge).
     */
    Node* getNode0() const override;

    /**
     * @brief Get pointer to node 1 (node at other end of the edge).
     * @return Pointer to node 1 (node at other end of the edge).
     */
    Node* getNode1() const override;
};

}  // namespace arch