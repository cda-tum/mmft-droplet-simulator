/**
 * @file DropletBoundary.h
 */
#pragma once

#include "Channel.h"
#include "ChannelPosition.h"
#include "Chip.h"

namespace sim {

/**
 * @brief Enum to specify in which state a droplet boundary currently is.
 */
enum class BoundaryState { NORMAL, WAIT_INFLOW, WAIT_OUTFLOW };

/**
 * @brief Class to specify a boundary of a droplet.
 */
class DropletBoundary {
  private:
    arch::ChannelPosition channelPosition;  ///< Channel position of the boundary.
    bool volumeTowardsNode0;                ///< Direction in which the volume of the boundary is located (true if it is towards node0).
    double flowRate;                        ///< Flow rate of the boundary (if <0 the boundary moves towards the droplet center, >0 otherwise).
    BoundaryState state;                    ///< Current status of the boundary

  public:
    /**
     * @brief Construct a new droplet boundary
     * @param channel Channel of the boundary.
     * @param position Position of the boundary within the channel.
     * @param volumeTowardsNode0 Direction in which the volume of the boundary is located (true if it is towards node0).
     * @param state State in which the boundary is in.
     */
    DropletBoundary(arch::Channel* channel, double position, bool volumeTowardsNode0, BoundaryState state);

    /**
     * @brief Get the channel position of the boundary.
     * @return The channel position.
     */
    arch::ChannelPosition& getChannelPosition();

    /**
     * @brief Get the flow rate of the boundary.
     * @return The flow rate of the boundary.
     */
    double getFlowRate() const;

    /**
     * @brief Get the direction in which the volume of the boundary is located.
     * @return true if the volume of the droplet lies between the boundary and node0 and false if it lies in the direction of node1.
     */
    bool isVolumeTowardsNode0() const;

    /**
     * @brief Get the state in which the boundary is currently in.
     * @return Current state of the boundary.
     */
    BoundaryState getState() const;

    /**
     * @brief Set the flow rate of the boundary.
     * @param flowRate Flow rate of the boundary.
     */
    void setFlowRate(double flowRate);

    /**
     * @brief Set the direction in which the volume of the boundary is located.
     * @param volumeTowardsNode0 Set to true if the droplet volume lies between the boundary and node0, otherwise set to false.
     */
    void setVolumeTowardsNode0(bool volumeTowardsNode0);

    /**
     * @brief Set the state of the boundary.
     * @param state State to which the boundary should be set.
     */
    void setState(BoundaryState state);

    /**
     * @brief Get the reference node of the boundary, which is the node that "touches" the droplet volume (i.e., if volumeTowardsNode0==true, then node0, otherwise node1)
     * @return Reference node of the boundary.
     */
    arch::Node* getReferenceNode();

    /**
     * @brief Get the opposite reference node of the boundary, which is the node that does not "touch" the droplet volume (i.e., if volumeTowardsNode0==true, then node1, otherwise node0)
     * @return Opposite reference node of the boundary.
     */
    arch::Node* getOppositeReferenceNode();

    /**
     * @brief Get the remaining volume between the boundary and the destination node, towards which the boundary is currently flowing.
     Hence, inflow boundaries return the volume between the boundary and the reference node, while outflow boundaries return the volume between the boundary and the opposite reference node.
     * @return The remaining volume of the boundary.
     */
    double getRemainingVolume();

    /**
     * @brief Get the volume between the boundary and the reference node.
     * @return The volume between the boundary and the reference node.
     */
    double getVolume();

    /**
     * @brief Get the flow rate of the channel the boundary is currently in. Does not necessarily have to match with the actual boundary flow rate.
     * @return The flow rate in the channel.
     */
    double getChannelFlowRate();

    /**
     * @brief Get the time at which the boundary will reach the end of the channel.
     * @return Time in s until the boundary will reach the end of the channel.
     */
    double getTime();

    /**
     * @brief Move boundary by the given timestep considering the given slip factor.
     * @param timeStep Timestep to which the boundary should be updated.
     */
    void moveBoundary(double timeStep);

    /**
     * @brief Update boundary wait state.
     * @param chip The simulated chip.
     */
    void updateWaitState(arch::Chip const& chip);

    /**
     * @brief Get if boundary is in wait state.
     * @return true if it is in wait state, false if not.
     */
    bool isInWaitState();
};

}  // namespace sim