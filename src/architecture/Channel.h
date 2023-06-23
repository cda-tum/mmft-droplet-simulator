/**
 * @file Channel.h
 */

#pragma once

#include "Edge.h"
#include "../nodalAnalysis/IResistance.h"
#include "Node.h"

namespace arch {

/**
 * @brief An enum to specify the type of channel.
 */
enum class ChannelType {
    NORMAL,    ///< A normal channel is the regular channel in which droplets can flow.
    BYPASS,    ///< A bypass channel allows droplets to bypass another channel e.g. if a droplet is trapped in that channel.
    CLOGGABLE  ///< A cloggable channel will be clogged during the time a droplet passes by one of its ends.
};

/**
 * @brief Class to specify a channel, which is a component of a chip in which droplet can flow.
 */
class Channel : public virtual Edge, public virtual nodal::IResistance {
  private:
    double height = 0;                       ///< Height of a channel in m.
    double width = 0;                        ///< Width of a channel in m.
    double length = 0;                       ///< Length of a channel in m.
    double dropletResistance = 0;            ///< Resistance of droplets inside the channel in Pas/L.
    double channelResistance = 0;            ///< Resistance of a channel without droplets in Pas/L.
    ChannelType type = ChannelType::NORMAL;  ///< What kind of channel it is.

  public:
    /**
     * @brief Constructor of a channel.
     * @param[in] id Id of the channel.
     * @param[in] node0 Node at one end of the channel.
     * @param[in] node1 Node at other end of the channel.
     * @param[in] height Height of the channel in m.
     * @param[in] width Width of the channel in m.
     * @param[in] length Length of the channel in m.
     * @param[in] type Type of the channel.
     */
    Channel(int id, Node* node0, Node* node1, double height, double width, double length, ChannelType type = ChannelType::NORMAL);

    /**
     * @brief Constructor of a channel.
     * @param[in] id Id of the channel.
     * @param[in] node0 Node at one end of the channel.
     * @param[in] node1 Node at other end of the channel.
     * @param[in] resistance Resistance of the channel in Pas/L.
     * @param[in] type Type of the channel.
     */
    Channel(int id, Node* node0, Node* node1, double resistance, ChannelType type = ChannelType::NORMAL);

    /**
     * @brief Set dimensions of a channel.
     * @param[in] width Width of a channel in m.
     * @param[in] height Height of a channel in m.
     * @param[in] length Length of a channel in m.
     */
    void setDimensions(double width, double height, double length);

    /**
     * @brief Set height of a channel.
     * @param height New height of channel in m.
     */
    void setHeight(double height);

    /**
     * @brief Set width of channel.
     * @param[in] width New width of a channel in m.
     */
    void setWidth(double width);

    /**
     * @brief Set length of channel.
     * @param[in] length New length of this channel in m.
     */
    void setLength(double length);

    /**
     * @brief Set resistance of a channel without droplets.
     * @param[in] channelResistance Resistance of a channel without droplets in Pas/L.
     */
    void setChannelResistance(double channelResistance);

    /**
     * @brief Set resistance caused by droplets within channel.
     * @param[in] dropletResistance Resistance caused by droplets within channel in Pas/L.
     */
    void setDropletResistance(double dropletResistance);

    /**
     * @brief Set which kind of channel it is.
     * @param[in] channelType Which kind of channel it is.
     */
    void setChannelType(ChannelType channelType);

    /**
     * @brief Add resistance caused by a droplet to droplet resistance of channel that is caused by all droplets currently in the channel.
     * @param[in] dropletResistance Resistance caused by a droplet in Pas/L.
     */
    void addDropletResistance(double dropletResistance);

    /**
     * @brief Returns the height of this channel.
     * @returns Height of channel in m.
     */
    double getHeight() const;

    /**
     * @brief Returns the width of this channel.
     * @returns Width of channel in m.
     */
    double getWidth() const;

    /**
     * @brief Returns the length of this channel.
     * @returns Length of channel in m.
     */
    double getLength() const;

    /**
     * @brief Returns area of a channel.
     * @returns Area in m^2.
     */
    double getArea() const;

    /**
     * @brief Calculates and returns volume of the channel.
     * @returns Volumne of a channel in m^3.
     */
    double getVolume() const;

    /**
     * @brief Calculates and returns pressure difference over a channel.
     * @returns Pressure in Pa.
     */
    double getPressure() const override;

    /**
     * @brief Calculate flow rate within the channel.
     * @returns Flow rate in m^3/s.
     */
    double getFlowRate() const override;

    /**
     * @brief Calculate and returns overall resistance caused by the channel itself and the droplets within the channel.
     * @returns Overall resistance in Pas/L.
     */
    double getResistance() const override;

    /**
     * @brief Returns resistance caused by the channel itself.
     * @returns Resistance caused by the channel itself in Pas/L.
     */
    double getChannelResistance() const;

    /**
     * @brief Returns resistance caused by the droplets within the channel.
     * @returns The resistance caused by the droplets within the channel in Pas/L.
     */
    double getDropletResistance() const;

    /**
     * @brief Returns the type of channel.
     * @returns What kind of channel it is.
     */
    ChannelType getChannelType() const;
};

}  // namespace arch
