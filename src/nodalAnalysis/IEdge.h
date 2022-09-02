/**
 * @file IEdge.h
 */

#pragma once

#include "INode.h"

namespace nodal {

/**
 * @brief Interface of an edge required for the nodal analysis. An edge is a connection between two nodes.
 */
class IEdge {
  public:
    /**
     * @brief Constructor of a connection between two nodes.
     */
    virtual ~IEdge() {}

    /**
     * @brief Function to return the node at one end of the edge.
     * @return Pointer to an INode.
     */
    virtual INode* getNode0() const = 0;

    /**
     * @brief Function to return the node at the other end of the edge.
     * @return Pointer to an INode.
     */
    virtual INode* getNode1() const = 0;
};

}  // namespace nodal