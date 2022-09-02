#include <stdexcept>
#include <vector>

#include "Channel.h"
#include "Chip.h"
#include "FlowRatePump.h"
#include "NodalAnalysis.h"
#include "Node.h"
#include "PressurePump.h"
#include "gtest/gtest.h"

TEST(Chip, testNetwork1) {
    // define network
    arch::Chip chip;
    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;
    int node3Id = 3;

    // pressure pump (voltage sources)
    auto v0 = chip.addPressurePump(nodeGroundId, node0Id, 1.0);

    // flowRate pump (current source)
    auto i0 = chip.addFlowRatePump(nodeGroundId, node2Id, 1.0);

    // channels
    auto c1 = chip.addChannel(node0Id, node1Id, 5, arch::ChannelType::NORMAL);
    auto c2 = chip.addChannel(node1Id, nodeGroundId, 10, arch::ChannelType::NORMAL);
    auto c3 = chip.addChannel(node2Id, node3Id, 5, arch::ChannelType::NORMAL);
    auto c4 = chip.addChannel(node3Id, nodeGroundId, 10, arch::ChannelType::NORMAL);

    ASSERT_THROW(chip.getGroundId(), std::invalid_argument);

    chip.addGround(nodeGroundId);

    // compute network
    std::unordered_map<int, std::tuple<nodal::INode*, int>> nodes;
    int matrixId = 0;
    for (auto& [nodeId, node] : chip.getNodes()) {
        // if node is ground node set the matrixId to -1
        if (nodeId == chip.getGroundId()) {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), -1));
        } else {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), matrixId));
            matrixId++;
        }
    }
    std::vector<nodal::IResistance*> channels;
    for (const auto& [key, channel] : chip.getChannels()) {
        channels.push_back(channel.get());
    }
    std::vector<nodal::IFlowRatePump*> flowRatePumps;
    for (const auto& [key, pump] : chip.getFlowRatePumps()) {
        flowRatePumps.push_back(pump.get());
    }
    std::vector<nodal::IPressurePump*> pressurePumps;
    for (const auto& [key, pump] : chip.getPressurePumps()) {
        pressurePumps.push_back(pump.get());
    }

    nodal::conductNodalAnalysis(nodes, channels, pressurePumps, flowRatePumps);

    const auto& chipNodes = chip.getNodes();
    const auto& chipPressurePump = chip.getPressurePumps();
    // check result
    const double errorTolerance = 1e-6;
    // pressure at nodes
    ASSERT_NEAR(chipNodes.at(node0Id)->getPressure(), 1.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node1Id)->getPressure(), 2.0 / 3.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node2Id)->getPressure(), 15.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node3Id)->getPressure(), 10.0, errorTolerance);
    // flow rate at pressure pumps
    ASSERT_NEAR(chipPressurePump.at(v0)->getFlowRate(), -0.2 / 3.0, errorTolerance);
}

TEST(Chip, testNetwork2) {
    // define network
    arch::Chip chip;
    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;
    int node3Id = 3;
    int node4Id = 4;

    // pressure pump (voltage sources)
    auto v0 = chip.addPressurePump(nodeGroundId, node0Id, 1.0);
    auto v1 = chip.addPressurePump(node4Id, nodeGroundId, 2.0);

    // flowRate pump (current source)
    auto i0 = chip.addFlowRatePump(nodeGroundId, node1Id, 1.0);

    // channels
    auto c1 = chip.addChannel(node0Id, node1Id, 5, arch::ChannelType::NORMAL);
    auto c2 = chip.addChannel(nodeGroundId, node1Id, 10, arch::ChannelType::NORMAL);
    auto c3 = chip.addChannel(node1Id, node2Id, 20, arch::ChannelType::NORMAL);
    auto c4 = chip.addChannel(node2Id, node3Id, 30, arch::ChannelType::NORMAL);

    chip.addGround(nodeGroundId);

    // compute network
    std::unordered_map<int, std::tuple<nodal::INode*, int>> nodes;
    int matrixId = 0;
    for (const auto& [nodeId, node] : chip.getNodes()) {
        // if node is ground node set the matrixId to -1
        if (nodeId == chip.getGroundId()) {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), -1));
        } else {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), matrixId));
            matrixId++;
        }
    }
    std::vector<nodal::IResistance*> channels;
    for (const auto& [key, channel] : chip.getChannels()) {
        channels.push_back(channel.get());
    }
    std::vector<nodal::IFlowRatePump*> flowRatePumps;
    for (const auto& [key, pump] : chip.getFlowRatePumps()) {
        flowRatePumps.push_back(pump.get());
    }
    std::vector<nodal::IPressurePump*> pressurePumps;
    for (const auto& [key, pump] : chip.getPressurePumps()) {
        pressurePumps.push_back(pump.get());
    }

    nodal::conductNodalAnalysis(nodes, channels, pressurePumps, flowRatePumps);

    const auto& chipNodes = chip.getNodes();
    const auto& chipPressurePump = chip.getPressurePumps();

    // check result
    const double errorTolerance = 1e-6;
    // pressure at nodes
    ASSERT_NEAR(chipNodes.at(node0Id)->getPressure(), 1.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node1Id)->getPressure(), 4.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node2Id)->getPressure(), 4.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node3Id)->getPressure(), 4.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node4Id)->getPressure(), -2.0, errorTolerance);
    // flow rate at pressure pumps
    ASSERT_NEAR(chipPressurePump.at(v0)->getFlowRate(), 0.6, errorTolerance);
    ASSERT_NEAR(chipPressurePump.at(v1)->getFlowRate(), 0.0, errorTolerance);
}

TEST(Chip, testNetwork3) {
    // define network
    arch::Chip chip;
    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;

    // pressure pump (voltage sources)
    auto v0 = chip.addPressurePump(node1Id, node0Id, 32.0);
    auto v1 = chip.addPressurePump(node2Id, nodeGroundId, 20.0);

    // flowRate pump (current source)

    // channels
    auto c1 = chip.addChannel(nodeGroundId, node0Id, 2, arch::ChannelType::NORMAL);
    auto c2 = chip.addChannel(node1Id, node2Id, 4, arch::ChannelType::NORMAL);
    auto c3 = chip.addChannel(node1Id, nodeGroundId, 8, arch::ChannelType::NORMAL);

    chip.addGround(nodeGroundId);

    // compute network
    std::unordered_map<int, std::tuple<nodal::INode*, int>> nodes;
    int matrixId = 0;
    for (const auto& [nodeId, node] : chip.getNodes()) {
        // if node is ground node set the matrixId to -1
        if (nodeId == chip.getGroundId()) {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), -1));
        } else {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), matrixId));
            matrixId++;
        }
    }
    std::vector<nodal::IResistance*> channels;
    for (const auto& [key, channel] : chip.getChannels()) {
        channels.push_back(channel.get());
    }
    std::vector<nodal::IFlowRatePump*> flowRatePumps;
    for (const auto& [key, pump] : chip.getFlowRatePumps()) {
        flowRatePumps.push_back(pump.get());
    }
    std::vector<nodal::IPressurePump*> pressurePumps;
    for (const auto& [key, pump] : chip.getPressurePumps()) {
        pressurePumps.push_back(pump.get());
    }

    nodal::conductNodalAnalysis(nodes, channels, pressurePumps, flowRatePumps);

    const auto& chipNodes = chip.getNodes();
    const auto& chipPressurePumps = chip.getPressurePumps();

    // check result
    const double errorTolerance = 1e-6;
    // pressure at nodes
    ASSERT_NEAR(chipNodes.at(node0Id)->getPressure(), 8.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node1Id)->getPressure(), -24.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node2Id)->getPressure(), -20.0, errorTolerance);
    // flow rate at pressure pumps
    ASSERT_NEAR(chipPressurePumps.at(v0)->getFlowRate(), -4.0, errorTolerance);
    ASSERT_NEAR(chipPressurePumps.at(v1)->getFlowRate(), 1.0, errorTolerance);
}

TEST(Chip, testNetwork4) {
    // define network
    arch::Chip chip;
    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;

    // pressure pump (voltage sources)
    auto v0 = chip.addPressurePump(node0Id, node1Id, 32.0);

    // flowRate pump (current source)
    auto i0 = chip.addFlowRatePump(node0Id, nodeGroundId, 20.0);

    // channels
    auto c1 = chip.addChannel(nodeGroundId, node0Id, 2, arch::ChannelType::NORMAL);
    auto c2 = chip.addChannel(node0Id, node1Id, 4, arch::ChannelType::NORMAL);
    auto c3 = chip.addChannel(node1Id, nodeGroundId, 8, arch::ChannelType::NORMAL);

    chip.addGround(nodeGroundId);

    // compute network
    std::unordered_map<int, std::tuple<nodal::INode*, int>> nodes;
    int matrixId = 0;
    for (const auto& [nodeId, node] : chip.getNodes()) {
        // if node is ground node set the matrixId to -1
        if (nodeId == chip.getGroundId()) {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), -1));
        } else {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), matrixId));
            matrixId++;
        }
    }
    std::vector<nodal::IResistance*> channels;
    for (const auto& [key, channel] : chip.getChannels()) {
        channels.push_back(channel.get());
    }
    std::vector<nodal::IFlowRatePump*> flowRatePumps;
    for (const auto& [key, pump] : chip.getFlowRatePumps()) {
        flowRatePumps.push_back(pump.get());
    }
    std::vector<nodal::IPressurePump*> pressurePumps;
    for (const auto& [key, pump] : chip.getPressurePumps()) {
        pressurePumps.push_back(pump.get());
    }

    nodal::conductNodalAnalysis(nodes, channels, pressurePumps, flowRatePumps);

    const auto& chipNodes = chip.getNodes();
    const auto& chipPressurePumps = chip.getPressurePumps();

    // check result
    const double errorTolerance = 1e-6;
    // pressure at nodes
    ASSERT_NEAR(chipNodes.at(node0Id)->getPressure(), -38.4, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node1Id)->getPressure(), -6.4, errorTolerance);
    // flow rate at pressure pumps
    ASSERT_NEAR(chipPressurePumps.at(v0)->getFlowRate(), -7.2, errorTolerance);
}

TEST(Chip, testNetwork5) {
    // define network
    arch::Chip chip;
    // nodes
    auto nodeGroundId = -1;
    auto node0Id = 0;
    auto node1Id = 1;
    auto node2Id = 2;

    // pressure pump (voltage sources)

    // flowRate pump (current source)
    auto i0 = chip.addFlowRatePump(node0Id, nodeGroundId, 1.0);
    auto i1 = chip.addFlowRatePump(node2Id, nodeGroundId, 1.5);

    // channels
    auto c1 = chip.addChannel(node0Id, node1Id, 5, arch::ChannelType::NORMAL);
    auto c2 = chip.addChannel(node1Id, node2Id, 7, arch::ChannelType::NORMAL);
    auto c3 = chip.addChannel(node1Id, nodeGroundId, 10, arch::ChannelType::NORMAL);

    chip.addGround(nodeGroundId);

    // compute network
    std::unordered_map<int, std::tuple<nodal::INode*, int>> nodes;
    int matrixId = 0;
    for (const auto& [nodeId, node] : chip.getNodes()) {
        // if node is ground node set the matrixId to -1
        if (nodeId == chip.getGroundId()) {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), -1));
        } else {
            nodes.insert_or_assign(nodeId, std::make_tuple(node.get(), matrixId));
            matrixId++;
        }
    }
    std::vector<nodal::IResistance*> channels;
    for (const auto& [key, channel] : chip.getChannels()) {
        channels.push_back(channel.get());
    }
    std::vector<nodal::IFlowRatePump*> flowRatePumps;
    for (const auto& [key, pump] : chip.getFlowRatePumps()) {
        flowRatePumps.push_back(pump.get());
    }
    std::vector<nodal::IPressurePump*> pressurePumps;
    for (const auto& [key, pump] : chip.getPressurePumps()) {
        pressurePumps.push_back(pump.get());
    }

    nodal::conductNodalAnalysis(nodes, channels, pressurePumps, flowRatePumps);

    const auto& chipNodes = chip.getNodes();
    const auto& chipPressurePumps = chip.getPressurePumps();

    // check result
    const double errorTolerance = 1e-6;
    // pressure at nodes
    ASSERT_NEAR(chipNodes.at(node0Id)->getPressure(), -30.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node1Id)->getPressure(), -25.0, errorTolerance);
    ASSERT_NEAR(chipNodes.at(node2Id)->getPressure(), -35.5, errorTolerance);
}

TEST(Chip, chipArchitectureDefinition) {
    // define network
    arch::Chip biochip;
    biochip.setName("testchip");
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;
    int node3Id = 3;
    int node4Id = 4;

    // pressure pump (voltage sources)
    int v0 = biochip.addPressurePump(node0Id, node1Id, 32.0);

    // flowRate pump (current source)
    int i0 = biochip.addFlowRatePump(node0Id, nodeGroundId, 20.0);

    // channels
    int c1 = biochip.addChannel(nodeGroundId, node0Id, 2, arch::ChannelType::NORMAL);
    int c2 = biochip.addChannel(node0Id, node1Id, 4, arch::ChannelType::BYPASS);
    int c3 = biochip.addChannel(node1Id, nodeGroundId, 8, arch::ChannelType::CLOGGABLE);

    ASSERT_EQ(biochip.getName(), "testchip");
    ASSERT_EQ(biochip.getPressurePumps().at(v0)->getNode0()->getId(), node0Id);
    ASSERT_EQ(biochip.getPressurePumps().at(v0)->getNode1()->getId(), node1Id);
    ASSERT_EQ(biochip.getPressurePumps().at(v0)->getPressure(), 32.0);
    ASSERT_EQ(biochip.getFlowRatePumps().at(i0)->getNode0()->getId(), node0Id);
    ASSERT_EQ(biochip.getFlowRatePumps().at(i0)->getNode1()->getId(), nodeGroundId);
    ASSERT_EQ(biochip.getFlowRatePumps().at(i0)->getFlowRate(), 20.0);
    ASSERT_EQ(biochip.getChannels().at(c1)->getNode0()->getId(), nodeGroundId);
    ASSERT_EQ(biochip.getChannels().at(c1)->getNode1()->getId(), node0Id);
    ASSERT_EQ(biochip.getChannels().at(c1)->getChannelType(), arch::ChannelType::NORMAL);
    ASSERT_EQ(biochip.getChannels().at(c2)->getNode0()->getId(), node0Id);
    ASSERT_EQ(biochip.getChannels().at(c2)->getNode1()->getId(), node1Id);
    ASSERT_EQ(biochip.getChannels().at(c2)->getChannelType(), arch::ChannelType::BYPASS);
    ASSERT_EQ(biochip.getChannels().at(c3)->getNode0()->getId(), node1Id);
    ASSERT_EQ(biochip.getChannels().at(c3)->getNode1()->getId(), nodeGroundId);
    ASSERT_EQ(biochip.getChannels().at(c3)->getChannelType(), arch::ChannelType::CLOGGABLE);
}

TEST(Chip, chipName) {
    // define network
    arch::Chip biochip;
    biochip.setName("testchip");
    ASSERT_EQ(biochip.getName(), "testchip");
}