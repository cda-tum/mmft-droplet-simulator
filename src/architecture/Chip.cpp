#include "Chip.h"

#include <memory>
#include <unordered_map>

#include "FlowRatePump.h"
#include "PressurePump.h"

namespace arch {

Chip::Chip() {}

Node* Chip::getOrAddNode(int nodeId) {
    auto result = nodes.insert({nodeId, std::make_unique<Node>(nodeId)});

    if (result.second) {
        // insertion happened and we have to add an additional entry into the network
        network.insert_or_assign(nodeId, std::vector<Channel*>{});
    }

    // return raw pointer to the node
    return result.first->second.get();
}

void Chip::visitNodes(int id, std::unordered_map<int, bool>& visitedNodes, std::unordered_map<int, bool>& visitedChannels) {
    const auto net = network.at(id);
    for (Channel* channel : net) {
        if (!(channel->getChannelType() == ChannelType::CLOGGABLE)) {
            visitedNodes.at(id) = true;
            if (visitedChannels[channel->getId()] == false) {
                visitedChannels.at(channel->getId()) = true;
                if (channel->getNode0()->getId() != id) {
                    visitNodes(channel->getNode0()->getId(), visitedNodes, visitedChannels);
                } else {
                    visitNodes(channel->getNode1()->getId(), visitedNodes, visitedChannels);
                }
            }
        }
    }
}

void Chip::setName(std::string name) {
    this->name = std::move(name);
}

std::string Chip::getName() const {
    return name;
}

int Chip::addChannel(int node0Id, int node1Id, double height, double width, double length, ChannelType type) {
    // create channel
    auto node0 = getOrAddNode(node0Id);
    auto node1 = getOrAddNode(node1Id);
    auto id = channels.size() + flowRatePumps.size() + pressurePumps.size();
    auto channel = std::make_unique<Channel>(id, node0, node1, height, width, length, type);

    // add to network as long as channel is still a valid pointer
    network.at(node0->getId()).push_back(channel.get());
    network.at(node1->getId()).push_back(channel.get());

    // add channel
    channels.insert_or_assign(id, std::move(channel));

    return id;
}

int Chip::addChannel(int node0Id, int node1Id, double resistance, ChannelType type) {
    // create channel
    auto node0 = getOrAddNode(node0Id);
    auto node1 = getOrAddNode(node1Id);
    auto id = channels.size() + flowRatePumps.size() + pressurePumps.size();
    auto channel = std::make_unique<Channel>(id, node0, node1, resistance, type);

    // add to network as long as channel is still a valid pointer
    network.at(node0->getId()).push_back(channel.get());
    network.at(node1->getId()).push_back(channel.get());

    // add channel
    channels.insert_or_assign(id, std::move(channel));

    return id;
}

int Chip::addFlowRatePump(int node0Id, int node1Id, double flowRate) {
    // create pump
    auto node0 = getOrAddNode(node0Id);
    auto node1 = getOrAddNode(node1Id);
    auto id = channels.size() + flowRatePumps.size() + pressurePumps.size();
    auto pump = std::make_unique<FlowRatePump>(id, node0, node1, flowRate);

    // add pump
    flowRatePumps.insert_or_assign(id, std::move(pump));

    return id;
}

int Chip::addPressurePump(int node0Id, int node1Id, double pressure) {
    // create pump
    auto node0 = getOrAddNode(node0Id);
    auto node1 = getOrAddNode(node1Id);
    auto id = channels.size() + flowRatePumps.size() + pressurePumps.size();
    auto pump = std::make_unique<PressurePump>(id, node0, node1, pressure);

    // add pump
    pressurePumps.insert_or_assign(id, std::move(pump));

    return id;
}

void Chip::addSink(int nodeId) {
    auto sink = getOrAddNode(nodeId);

    //insert sink into sinks (does nothing if sink is already present in sinks)
    sinks.insert(sink);
}

void Chip::addGround(int nodeId) {
    auto groundNode = getOrAddNode(nodeId);

    //insert ground node into groundNodes (does nothing if ground node is already present in groundNodes)
    groundNodes.insert(groundNode);
}

bool Chip::isSink(int nodeId) const {
    return sinks.count(nodes.at(nodeId).get()) == 1;
}

std::set<int> Chip::getGroundIds() const {
    if (groundNodes.empty()) {
        throw std::invalid_argument("Ground node not defined.");
    }

    std::set<int> groundIds;
    for (auto groundNode : groundNodes) {
        groundIds.insert(groundNode->getId());
    }

    return groundIds;
}

std::set<Node*> Chip::getGroundNodes() const {
    return groundNodes;
}

bool Chip::hasNode(int nodeId) const {
    return nodes.count(nodeId);
}

Node* Chip::getNode(int nodeId) const {
    try {
        return nodes.at(nodeId).get();
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Node with ID " + std::to_string(nodeId) + " does not exist.");
    }
}

Channel* Chip::getChannel(int channelId) const {
    try {
        return channels.at(channelId).get();
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Channel with ID " + std::to_string(channelId) + " does not exist.");
    }
}

FlowRatePump* Chip::getFlowRatePump(int flowRatePumpId) const {
    try {
        return flowRatePumps.at(flowRatePumpId).get();
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Flow rate pump with ID " + std::to_string(flowRatePumpId) + " does not exist.");
    }
}

PressurePump* Chip::getPressurePump(int pressurePumpId) const {
    try {
        return pressurePumps.at(pressurePumpId).get();
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Pressure pump with ID " + std::to_string(pressurePumpId) + " does not exist.");
    }
}

const std::unordered_map<int, std::unique_ptr<Channel>>& Chip::getChannels() const {
    return channels;
}

const std::unordered_map<int, std::unique_ptr<Node>>& Chip::getNodes() const {
    return nodes;
}

const std::unordered_map<int, std::unique_ptr<FlowRatePump>>& Chip::getFlowRatePumps() const {
    return flowRatePumps;
}

const std::unordered_map<int, std::unique_ptr<PressurePump>>& Chip::getPressurePumps() const {
    return pressurePumps;
}

const std::vector<Channel*>& Chip::getChannelsAtNode(int nodeId) const {
    try {
        return network.at(nodeId);
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Node with ID " + std::to_string(nodeId) + " does not exist.");
    }
}

bool Chip::isNetworkValid() {
    // checks if all nodes and channels are connected to ground (if channel network is one graph)
    std::unordered_map<int, bool> visitedNodes;
    std::unordered_map<int, bool> visitedChannels;

    if (nodes.size() == 0) {
        throw std::invalid_argument("No nodes in network.");
    }

    for (auto const& [k, v] : channels) {
        if (v->getLength() <= 0) {
            throw std::invalid_argument("Channel " + std::to_string(k) + ": length is <= 0.");
        }
        if (v->getHeight() <= 0) {
            throw std::invalid_argument("Channel " + std::to_string(k) + ": height is <= 0.");
        }
        if (v->getWidth() <= 0) {
            throw std::invalid_argument("Channel " + std::to_string(k) + ": width is <= 0.");
        }
    }

    for (auto const& [k, v] : nodes) {
        visitedNodes[k] = false;
    }
    for (auto const& [k, v] : channels) {
        visitedChannels[k] = false;
    }

    visitNodes(*getGroundIds().begin(), visitedNodes, visitedChannels);

    std::string errorNodes = "";
    for (auto const& [k, v] : nodes) {
        if (visitedNodes[k] == false) {
            errorNodes.append(" " + std::to_string(k));
        }
    }
    std::string errorChannels = "";
    for (auto const& [k, v] : channels) {
        if (visitedChannels[k] == false) {
            errorChannels.append(" " + std::to_string(k));
        }
    }

    if (errorNodes.length() != 0 || errorChannels.length() != 0) {
        throw std::invalid_argument("Chip is invalid. The following nodes are not connected to ground: " + errorNodes + ". The following channels are not connected to ground: " + errorChannels);
        return false;
    }
    return true;
}

}  // namespace arch