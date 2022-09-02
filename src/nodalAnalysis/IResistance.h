/**
 * @file IResistance.h
 */
#pragma once

#include "IEdge.h"

namespace nodal {

/**
 * @brief Interface for edges that have resistance values required for the nodal analysis.
 */
class IResistance : public virtual IEdge {
  public:
    /**
     * @brief Virtual constructor for edges that have resistance values.
     */
    virtual ~IResistance() {}

    /**
     * @brief Function that returns resistance value.
     * @return Resistance value of the edge.
     */
    virtual double getResistance() const = 0;
};

}  // namespace nodal