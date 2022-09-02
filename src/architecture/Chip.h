/**
 * @file Chip.h
 */
#pragma once

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "Channel.h"
#include "FlowRatePump.h"
#include "Node.h"
#include "PressurePump.h"

namespace arch {

/**
 * @brief Class to specify a microfluidic chip with all of its components.
 */
class Chip {
  private:
    std::string name;                                                      ///< Name of the chip.
    std::unordered_map<int, std::unique_ptr<Node>> nodes;                  ///< Nodes the network of the chip consists of.
    std::set<Node*> sinks;                                                 ///< Ids of nodes that are sinks.
    Node* groundNode = nullptr;                                            ///< Pointer to the ground node.
    std::unordered_map<int, std::unique_ptr<Channel>> channels;            ///< Map of ids and channel pointer of all channels the chip consists of.
    std::unordered_map<int, std::unique_ptr<PressurePump>> pressurePumps;  ///< Map of ids and pressure pump pointers of all pressure pumps the chip consists of.
    std::unordered_map<int, std::unique_ptr<FlowRatePump>> flowRatePumps;  ///< Map of ids of flow rate pump pointers of all flow rate pumps the chip consists of.
    std::unordered_map<int, std::vector<Channel*>> network;                ///< Network of nodes and corresponding channels at these nodes on the chip.

    /**
     * @brief To add components (vertices) to the chip, the nodes of these vertices need to be specified. This function tries to find the specified node or creates a new one with this id if it does not exist yet and returns it.
     * @param[in] nodeId Id of the node to be returned.
     * @return Node with the given id.
     */
    Node* getOrAddNode(int nodeId);

    /**
     * @brief Goes through network and sets all nodes and channels that are visited to true.
     * @param[in] id Id of the node that is visited.
     * @param[in, out] visitedNodes Reference to a map that stores which nodes have already been visited.
     * @param[in, out] visitedChannels Reference to a map that stores which channels have already been visited.
     */
    void visitNodes(int id, std::unordered_map<int, bool>& visitedNodes, std::unordered_map<int, bool>& visitedChannels);

  public:
    /**
     * @brief Constructor of the chip
     */
    Chip();

    /**
     * @brief Sets the name of the chip.
     * @param[in] name Name of the chip.
     */
    void setName(std::string name);

    /**
     * @brief Returns the name of the chip.
     * @return Name of the chip.
     */
    std::string getName() const;

    /**
     * @brief Adds a new channel to the chip.
     * @param[in] node0Id Id of the node at one end of the channel.
     * @param[in] node1Id Id of the node at the other end of the channel.
     * @param[in] height Height of the channel in m.
     * @param[in] width Width of the channel in m.
     * @param[in] length Length of the channel in m.
     * @param[in] type What kind of channel it is.
     * @return Id of the newly created channel.
     */
    int addChannel(int node0Id, int node1Id, double height, double width, double length, ChannelType type);

    /**
     * @brief Adds a new channel to the chip.
     * @param[in] node0Id Id of the node at one end of the channel.
     * @param[in] node1Id Id of the node at the other end of the channel.
     * @param[in] resistance Resistance of the channel in Pas/L.
     * @param[in] type What kind of channel it is.
     * @return Id of the newly created channel.
     */
    int addChannel(int node0Id, int node1Id, double resistance, ChannelType type);

    /**
     * @brief Adds a new flow rate pump to the chip.
     * @param[in] node0Id Id of the node at one end of the flow rate pump.
     * @param[in] node1Id Id of the node at the other end of the flow rate pump.
     * @param[in] flowRate Volumetric flow rate of the pump in m^3/s.
     * @return Id of the newly created flow rate pump.
     */
    int addFlowRatePump(int node0Id, int node1Id, double flowRate);

    /**
     * @brief Adds a new pressure pump to the chip.
     * @param[in] node0Id Id of the node at one end of the pressure pump.
     * @param[in] node1Id Id of the node at the other end of the pressure pump.
     * @param[in] pressure Pressure of the pump in Pas/L.
     * @return Id of the newly created pressure pump.
     */
    int addPressurePump(int node0Id, int node1Id, double pressure);

    /**
     * @brief Specifies a node as sink.
     * @param[in] nodeId Id of the node that is a sink.
     */
    void addSink(int nodeId);

    /**
     * @brief Adds or sets a node as the ground node, i.e., this node has a pressure value of 0 and acts as a reference node for all other nodes.
     * @param[in] nodeId Id of the node that should be the ground node of the network.
     */
    void addGround(int nodeId);

    /**
     * @brief Checks and returns if a node is a sink.
     * @param[in] nodeId Id of the node that should be checked.
     * @return If the node with the specified id is a sink.
     */
    bool isSink(int nodeId) const;

    /**
     * @brief Returns the id of the ground node.
     * @return Id of the ground node.
     */
    int getGroundId() const;

    /**
     * @brief Returns a pointer to the ground node.
     * @return Pointer to the ground node.
     */
    Node* getGroundNode() const;

    /**
     * @brief Checks if a node with the specified id exists in the network.
     * @param[in] nodeId Id of the node to check.
     * @return If such a node exists.
     */
    bool hasNode(int nodeId) const;

    /**
     * @brief Get pointer to node with the specified id.
     * @param[in] nodeId Id of the node to get.
     * @returns Pointer to the node with this id.
     */
    Node* getNode(int nodeId) const;

    /**
     * @brief Get pointer to channel with the specified id.
     * @param[in] channelId Id of the channel to get.
     * @return Pointer to the channel with this id.
     */
    Channel* getChannel(int channelId) const;

    /**
     * @brief Get pointer to flow rate pump with the specified id.
     * @param[in] flowRatePumpId Id of the flow rate pump to get.
     * @return Pointer to the flow rate pump with this id.
     */
    FlowRatePump* getFlowRatePump(int flowRatePumpId) const;

    /**
     * @brief Get pointer to pressure pump with the specified id.
     * @param[in] pressurePumpId Id of the pressure pump to get.
     * @return Pointer to the pressure pump with this id.
     */
    PressurePump* getPressurePump(int pressurePumpId) const;

    /**
     * @brief Get a map of all channels of the chip.
     @return Map that consists of the channel ids and pointers to the corresponding channels.
     */
    const std::unordered_map<int, std::unique_ptr<Channel>>& getChannels() const;

    /**
     * @brief Get a map of all nodes of the chip.
     * @return Map that consists of the node ids and pointers to the corresponding nodes.
     */
    const std::unordered_map<int, std::unique_ptr<Node>>& getNodes() const;

    /**
     * @brief Get a map of all flow rate pumps of the chip.
     * @return Map that consists of the flow rate pump ids and pointers to the corresponding flow rate pumps.
     */
    const std::unordered_map<int, std::unique_ptr<FlowRatePump>>& getFlowRatePumps() const;

    /**
     * @brief Get a map of all pressure pumps of the chip.
     * @return Map that consists of the pressure pump ids and pointers to the corresponding pressure pumps.
     */
    const std::unordered_map<int, std::unique_ptr<PressurePump>>& getPressurePumps() const;

    /**
     * @brief Get a map of all channels at a specific node.
     * @param[in] nodeId Id of the node at which the adherent channels should be returned.
     * @return Vector of pointers to channels adherent to this node.
     */
    const std::vector<Channel*>& getChannelsAtNode(int nodeId) const;

    /**
     * @brief Checks if chip network is valid.
     * @return If the network is valid.
     */
    bool isNetworkValid();
};

}  // namespace arch