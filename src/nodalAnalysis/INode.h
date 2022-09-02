/**
 * @file INode.h
 */

#pragma once

namespace nodal {

/**
 * @brief Interface for an node specifying functions required for the nodal analysis.
 */
class INode {
  public:
    /**
     * @brief Constructor of an INode.
     */
    virtual ~INode() {}

    /**
     * @brief Function to get the id of an node.
     * @return Id of the node.
     */
    virtual int getId() const = 0;

    /**
     * @brief Function to set the pressure level at a node.
     * @param[in] pressure Pressure level at a node in Pa.
     */
    virtual void setPressure(double pressure) = 0;
};

}  // namespace nodal