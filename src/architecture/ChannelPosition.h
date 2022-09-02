/**
 * @file ChannelPosition.h
 */

#pragma once

#include "Channel.h"

namespace arch {

/**
 * @brief Class to specify the boundary position of one end of a droplet.
 */
class ChannelPosition {
  private:
    Channel* channel;  ///< Channel in which one end of droplet currently is.
    double position;   ///< Exact relative position (between 0.0 and 1.0) within the channel.

  public:
    /**
     * @brief Constructor to create the position of one end of a droplet.
     * @param[in] channel Channel in which this end of the droplet currently is.
     * @param[in] position Relative position (between 0.0 and 1.0) of the droplet end in this channel.
     */
    ChannelPosition(Channel* channel, double position);

    /**
     * @brief Change the channel of the channel position (at which one end of the droplet currently is).
     * @param[in] channel New channel to which the position should be set.
     */
    void setChannel(Channel* const channel);

    /**
     * @brief Reset relative position.
     * @param[in] position Relative position (between 0.0 and 1.0) within a channel.
     */
    void setPosition(double position);

    /**
     * @brief Add the volume shift to the current position.
     * @param[in] volumeShift Shift of the volume in flow direction in L.
     */
    void addToPosition(double volumeShift);

    /**
     * @brief Returns pointer to channel in which this end of the droplet currently is.
     * @return Pointer to channel at which this end of the droplet currently is.
     */
    Channel* getChannel() const;

    /**
     * @brief Returns relative position within the channel.
     * @return Relative position (between 0.0 and 1.0) at which this end of the droplet currently is.
     */
    double getPosition() const;

    /**
     * @brief Returns absolute position within channel in m.
     * @return Absolute position in m.
     */
    double getAbsolutePosition() const;

    /**
     * @brief Calculates and returns volume towards node 0.
     * @return Volume towards node 0 in L.
     */
    double getVolume0() const;

    /**
     * @brief Calculates and returns volume towards node 1.
     * @return Volume towards node 1 in L.
     */
    double getVolume1() const;
};

}  // namespace arch