#include "Results.h"

#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Simulator.h"
#include "gtest/gtest.h"

TEST(Results, allResultValues) {
    //--- API ---
    droplet::Simulator sim;

    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;
    int node3Id = 3;
    int node4Id = 4;

    // flowRate pump
    auto flowRate = 3e-11;
    auto pump = sim.addFlowRatePump(nodeGroundId, node0Id, flowRate);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    auto c1 = sim.addChannel(node0Id, node1Id, cHeight, cWidth, cLength);
    auto c2 = sim.addChannel(node1Id, node2Id, cHeight, cWidth, cLength);
    auto c3 = sim.addChannel(node2Id, node3Id, cHeight, cWidth, cLength);
    auto c4 = sim.addChannel(node2Id, node4Id, cHeight, cWidth, cLength);
    auto c5 = sim.addChannel(node3Id, node4Id, cHeight, cWidth, cLength);
    auto c6 = sim.addChannel(node4Id, nodeGroundId, cHeight, cWidth, cLength);

    //--- sink ---
    sim.addSink(-1);
    //--- ground ---
    sim.addGround(-1);

    // fluids
    auto fluid0 = sim.addFluid(1e-3, 1e3);
    auto fluid1 = sim.addFluid(3e-3, 1e3);
    //--- continuousPhase ---
    sim.setContinuousPhase(fluid0);

    // droplet
    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    // check if chip is valid
    sim.checkChipValidity();

    // simulate
    droplet::SimulationResult result = sim.simulate();

    ASSERT_EQ(result.chip.name, "");

    ASSERT_EQ(result.chip.flowRatePumps.at(pump).id, pump);
    ASSERT_EQ(result.chip.flowRatePumps.at(pump).node0Id, nodeGroundId);
    ASSERT_EQ(result.chip.flowRatePumps.at(pump).node1Id, node0Id);
    ASSERT_EQ(result.chip.flowRatePumps.at(pump).flowRate, flowRate);

    ASSERT_EQ(result.chip.channels.at(c1).id, c1);
    ASSERT_EQ(result.chip.channels.at(c1).name, "");
    ASSERT_EQ(result.chip.channels.at(c1).node0Id, node0Id);
    ASSERT_EQ(result.chip.channels.at(c1).node1Id, node1Id);
    ASSERT_EQ(result.chip.channels.at(c1).width, cWidth);
    ASSERT_EQ(result.chip.channels.at(c1).height, cHeight);
    ASSERT_EQ(result.chip.channels.at(c1).length, cLength);
    ASSERT_EQ(result.chip.channels.at(c1).type, droplet::ChannelType::NORMAL);
    ASSERT_EQ(result.chip.channels.at(c2).id, c2);
    ASSERT_EQ(result.chip.channels.at(c2).name, "");
    ASSERT_EQ(result.chip.channels.at(c2).node0Id, node1Id);
    ASSERT_EQ(result.chip.channels.at(c2).node1Id, node2Id);
    ASSERT_EQ(result.chip.channels.at(c2).width, cWidth);
    ASSERT_EQ(result.chip.channels.at(c2).height, cHeight);
    ASSERT_EQ(result.chip.channels.at(c2).length, cLength);
    ASSERT_EQ(result.chip.channels.at(c2).type, droplet::ChannelType::NORMAL);
    ASSERT_EQ(result.chip.channels.at(c3).id, c3);
    ASSERT_EQ(result.chip.channels.at(c3).name, "");
    ASSERT_EQ(result.chip.channels.at(c3).node0Id, node2Id);
    ASSERT_EQ(result.chip.channels.at(c3).node1Id, node3Id);
    ASSERT_EQ(result.chip.channels.at(c3).width, cWidth);
    ASSERT_EQ(result.chip.channels.at(c3).height, cHeight);
    ASSERT_EQ(result.chip.channels.at(c3).length, cLength);
    ASSERT_EQ(result.chip.channels.at(c3).type, droplet::ChannelType::NORMAL);
    ASSERT_EQ(result.chip.channels.at(c4).id, c4);
    ASSERT_EQ(result.chip.channels.at(c4).name, "");
    ASSERT_EQ(result.chip.channels.at(c4).node0Id, node2Id);
    ASSERT_EQ(result.chip.channels.at(c4).node1Id, node4Id);
    ASSERT_EQ(result.chip.channels.at(c4).width, cWidth);
    ASSERT_EQ(result.chip.channels.at(c4).height, cHeight);
    ASSERT_EQ(result.chip.channels.at(c4).length, cLength);
    ASSERT_EQ(result.chip.channels.at(c4).type, droplet::ChannelType::NORMAL);
    ASSERT_EQ(result.chip.channels.at(c5).id, c5);
    ASSERT_EQ(result.chip.channels.at(c5).name, "");
    ASSERT_EQ(result.chip.channels.at(c5).node0Id, node3Id);
    ASSERT_EQ(result.chip.channels.at(c5).node1Id, node4Id);
    ASSERT_EQ(result.chip.channels.at(c5).width, cWidth);
    ASSERT_EQ(result.chip.channels.at(c5).height, cHeight);
    ASSERT_EQ(result.chip.channels.at(c5).length, cLength);
    ASSERT_EQ(result.chip.channels.at(c5).type, droplet::ChannelType::NORMAL);
    ASSERT_EQ(result.chip.channels.at(c6).id, c6);
    ASSERT_EQ(result.chip.channels.at(c6).name, "");
    ASSERT_EQ(result.chip.channels.at(c6).node0Id, node4Id);
    ASSERT_EQ(result.chip.channels.at(c6).node1Id, nodeGroundId);
    ASSERT_EQ(result.chip.channels.at(c6).width, cWidth);
    ASSERT_EQ(result.chip.channels.at(c6).height, cHeight);
    ASSERT_EQ(result.chip.channels.at(c6).length, cLength);
    ASSERT_EQ(result.chip.channels.at(c6).type, droplet::ChannelType::NORMAL);

    ASSERT_EQ(result.fluids.at(fluid0).id, fluid0);
    ASSERT_EQ(result.fluids.at(fluid0).name, "");
    ASSERT_EQ(result.fluids.at(fluid0).viscosity, 1e-3);
    ASSERT_EQ(result.fluids.at(fluid0).density, 1e3);
    ASSERT_EQ(result.fluids.at(fluid0).concentration, 0.0);
    ASSERT_EQ(result.fluids.at(fluid1).id, fluid1);
    ASSERT_EQ(result.fluids.at(fluid1).name, "");
    ASSERT_EQ(result.fluids.at(fluid1).viscosity, 3e-3);
    ASSERT_EQ(result.fluids.at(fluid1).density, 1e3);
    ASSERT_EQ(result.fluids.at(fluid1).concentration, 0.0);

    ASSERT_EQ(result.droplets.at(droplet0).id, droplet0);
    ASSERT_EQ(result.droplets.at(droplet0).name, "");
    // ASSERT_EQ(result.droplets.at(droplet0).fluid.id, result.fluids.at(fluid1).id);
    // ASSERT_EQ(result.droplets.at(droplet0).fluid.viscosity, result.fluids.at(fluid1).viscosity);
    // ASSERT_EQ(result.droplets.at(droplet0).fluid.density, result.fluids.at(fluid1).density);
    // ASSERT_EQ(result.droplets.at(droplet0).fluid.concentration, result.fluids.at(fluid1).concentration);
    ASSERT_EQ(result.droplets.at(droplet0).volume, dropletVolume);

    ASSERT_NEAR(result.states.at(0).time, 0.000000, 5e-7);
    ASSERT_NEAR(result.states.at(1).time, 0.000000, 5e-7);
    ASSERT_NEAR(result.states.at(2).time, 0.033203, 5e-7);
    ASSERT_NEAR(result.states.at(3).time, 0.044922, 5e-7);
    ASSERT_NEAR(result.states.at(4).time, 0.111328, 5e-7);
    ASSERT_NEAR(result.states.at(5).time, 0.125391, 5e-7);
    ASSERT_NEAR(result.states.at(6).time, 0.239941, 5e-7);
    ASSERT_NEAR(result.states.at(7).time, 0.254778, 5e-7);
    ASSERT_NEAR(result.states.at(8).time, 0.321184, 5e-7);

    ASSERT_NEAR(result.states.at(0).pressures.at(0), 602.237537, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(1).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(2).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(3).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(4).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(5).pressures.at(0), 630.802163, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(1), 466.555562, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(2), 302.308961, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(3), 233.277781, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(6).pressures.at(0), 630.802163, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(1), 466.555562, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(2), 302.308961, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(3), 233.277781, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(7).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(2), 347.655305, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(3), 292.906438, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(4), 238.157571, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(8).pressures.at(0), 602.237537, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(0).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(1).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(2).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(3).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(4).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(5).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(3), 0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(4), 0.0000000000173913, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(5), 0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(6).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(3), 0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(4), 0.0000000000173913, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(5), 0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(7).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(8).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(1), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(3), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(5), 0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(6), 0.00000000003, 5e-17);

    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).headPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(1).dropletPositions.at(droplet0).headPosition.position.position, 0.575000, 5e-7);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).tailPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(1).dropletPositions.at(droplet0).tailPosition.position.position, 0.425000, 5e-7);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).headPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(2).dropletPositions.at(droplet0).headPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).tailPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(2).dropletPositions.at(droplet0).tailPosition.position.position, 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).headPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(3).dropletPositions.at(droplet0).headPosition.position.position, 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).tailPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(3).dropletPositions.at(droplet0).tailPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).headPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(4).dropletPositions.at(droplet0).headPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).tailPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(4).dropletPositions.at(droplet0).tailPosition.position.position, 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).headPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(5).dropletPositions.at(droplet0).headPosition.position.position, 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).tailPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(5).dropletPositions.at(droplet0).tailPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).headPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(6).dropletPositions.at(droplet0).headPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).tailPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(6).dropletPositions.at(droplet0).tailPosition.position.position, 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).headPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(7).dropletPositions.at(droplet0).headPosition.position.position, 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).tailPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(7).dropletPositions.at(droplet0).tailPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    ASSERT_EQ(result.continuousPhaseId, fluid0);

    ASSERT_EQ(result.maximalAdaptiveTimeStep, 0);

    ASSERT_EQ(result.resistanceModel, 0);
}

TEST(Results, inverseDirectionChannels) {
    //--- API ---
    droplet::Simulator sim;

    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;
    int node3Id = 3;
    int node4Id = 4;

    // flowRate pump
    auto flowRate = 3e-11;
    auto pump = sim.addFlowRatePump(nodeGroundId, node0Id, flowRate);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    auto c1 = sim.addChannel(node1Id, node0Id, cHeight, cWidth, cLength);
    auto c2 = sim.addChannel(node2Id, node1Id, cHeight, cWidth, cLength);
    auto c3 = sim.addChannel(node3Id, node2Id, cHeight, cWidth, cLength);
    auto c4 = sim.addChannel(node4Id, node2Id, cHeight, cWidth, cLength);
    auto c5 = sim.addChannel(node4Id, node3Id, cHeight, cWidth, cLength);
    auto c6 = sim.addChannel(nodeGroundId, node4Id, cHeight, cWidth, cLength);

    //--- sink ---
    sim.addSink(-1);
    //--- ground ---
    sim.addGround(-1);

    // fluids
    auto fluid0 = sim.addFluid(1e-3, 1e3);
    auto fluid1 = sim.addFluid(3e-3, 1e3);
    //--- continuousPhase ---
    sim.setContinuousPhase(fluid0);

    // droplet
    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    // check if chip is valid
    sim.checkChipValidity();

    // simulate
    droplet::SimulationResult result = sim.simulate();

    ASSERT_NEAR(result.states.at(0).time, 0.000000, 5e-7);
    ASSERT_NEAR(result.states.at(1).time, 0.000000, 5e-7);
    ASSERT_NEAR(result.states.at(2).time, 0.033203, 5e-7);
    ASSERT_NEAR(result.states.at(3).time, 0.044922, 5e-7);
    ASSERT_NEAR(result.states.at(4).time, 0.111328, 5e-7);
    ASSERT_NEAR(result.states.at(5).time, 0.125391, 5e-7);
    ASSERT_NEAR(result.states.at(6).time, 0.239941, 5e-7);
    ASSERT_NEAR(result.states.at(7).time, 0.254778, 5e-7);
    ASSERT_NEAR(result.states.at(8).time, 0.321184, 5e-7);

    ASSERT_NEAR(result.states.at(0).pressures.at(0), 602.237537, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(1).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(2).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(3).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(4).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(5).pressures.at(0), 630.802163, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(1), 466.555562, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(2), 302.308961, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(3), 233.277781, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(6).pressures.at(0), 630.802163, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(1), 466.555562, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(2), 302.308961, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(3), 233.277781, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(7).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(2), 347.655305, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(3), 292.906438, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(4), 238.157571, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(8).pressures.at(0), 602.237537, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(0).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(1).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(2).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(3).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(4).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(5).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(3), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(4), -0.0000000000173913, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(5), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(6).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(3), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(4), -0.0000000000173913, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(5), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(7).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(6), -0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(8).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(2), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(4), -0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(6), -0.00000000003, 5e-17);

    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).headPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(1).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.425000, 5e-7);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).tailPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(1).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.575000, 5e-7);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).headPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(2).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).tailPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(2).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).headPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(3).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).tailPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(3).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).headPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(4).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).tailPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(4).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).headPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(5).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).tailPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(5).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).headPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(6).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).tailPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(6).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).headPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(7).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).tailPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(7).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    ASSERT_EQ(result.continuousPhaseId, fluid0);

    ASSERT_EQ(result.maximalAdaptiveTimeStep, 0);

    ASSERT_EQ(result.resistanceModel, 0);
}

TEST(Results, mixedDirectionChannels) {
    //--- API ---
    droplet::Simulator sim;

    // nodes
    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;
    int node2Id = 2;
    int node3Id = 3;
    int node4Id = 4;

    // flowRate pump
    auto flowRate = 3e-11;
    auto pump = sim.addFlowRatePump(nodeGroundId, node0Id, flowRate);

    // channels
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    auto c1 = sim.addChannel(node1Id, node0Id, cHeight, cWidth, cLength);
    auto c2 = sim.addChannel(node1Id, node2Id, cHeight, cWidth, cLength);
    auto c3 = sim.addChannel(node3Id, node2Id, cHeight, cWidth, cLength);
    auto c4 = sim.addChannel(node2Id, node4Id, cHeight, cWidth, cLength);
    auto c5 = sim.addChannel(node4Id, node3Id, cHeight, cWidth, cLength);
    auto c6 = sim.addChannel(node4Id, nodeGroundId, cHeight, cWidth, cLength);

    //--- sink ---
    sim.addSink(-1);
    //--- ground ---
    sim.addGround(-1);

    // fluids
    auto fluid0 = sim.addFluid(1e-3, 1e3);
    auto fluid1 = sim.addFluid(3e-3, 1e3);
    //--- continuousPhase ---
    sim.setContinuousPhase(fluid0);

    // droplet
    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    // check if chip is valid
    sim.checkChipValidity();

    // simulate
    droplet::SimulationResult result = sim.simulate();

    ASSERT_NEAR(result.states.at(0).time, 0.000000, 5e-7);
    ASSERT_NEAR(result.states.at(1).time, 0.000000, 5e-7);
    ASSERT_NEAR(result.states.at(2).time, 0.033203, 5e-7);
    ASSERT_NEAR(result.states.at(3).time, 0.044922, 5e-7);
    ASSERT_NEAR(result.states.at(4).time, 0.111328, 5e-7);
    ASSERT_NEAR(result.states.at(5).time, 0.125391, 5e-7);
    ASSERT_NEAR(result.states.at(6).time, 0.239941, 5e-7);
    ASSERT_NEAR(result.states.at(7).time, 0.254778, 5e-7);
    ASSERT_NEAR(result.states.at(8).time, 0.321184, 5e-7);

    ASSERT_NEAR(result.states.at(0).pressures.at(0), 602.237537, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(0).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(1).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(1).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(2).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(2).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(3).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(3).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(4).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(4).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(5).pressures.at(0), 630.802163, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(1), 466.555562, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(2), 302.308961, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(3), 233.277781, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(5).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(6).pressures.at(0), 630.802163, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(1), 466.555562, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(2), 302.308961, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(3), 233.277781, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(6).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(7).pressures.at(0), 676.148507, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(1), 511.901906, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(2), 347.655305, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(3), 292.906438, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(4), 238.157571, 5e-7);
    ASSERT_NEAR(result.states.at(7).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(8).pressures.at(0), 602.237537, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(1), 437.990936, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(2), 273.744335, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(3), 218.995468, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(4), 164.246601, 5e-7);
    ASSERT_NEAR(result.states.at(8).pressures.at(-1), 0.000000, 5e-7);

    ASSERT_NEAR(result.states.at(0).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(0).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(1).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(1).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(2).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(2).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(3).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(3).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(4).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(4).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(5).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(3), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(4), 0.0000000000173913, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(5), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(5).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(6).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(3), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(4), 0.0000000000173913, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(5), -0.0000000000126087, 5e-17);
    ASSERT_NEAR(result.states.at(6).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(7).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(7).flowRates.at(6), 0.00000000003, 5e-17);

    ASSERT_NEAR(result.states.at(8).flowRates.at(0), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(1), -0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(2), 0.00000000003, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(3), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(4), 0.00000000002, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(5), -0.00000000001, 5e-17);
    ASSERT_NEAR(result.states.at(8).flowRates.at(6), 0.00000000003, 5e-17);

    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).headPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(1).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.425000, 5e-7);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).tailPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(1).dropletPositions.at(droplet0).tailPosition.position.position, 1 - 0.575000, 5e-7);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(1).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).headPosition.position.channelId, 1);
    // ASSERT_NEAR(result.states.at(2).dropletPositions.at(droplet0).headPosition.position.position, 1 - 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).headPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).tailPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(2).dropletPositions.at(droplet0).tailPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(2).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).headPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(3).dropletPositions.at(droplet0).headPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).headPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).tailPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(3).dropletPositions.at(droplet0).tailPosition.position.position, 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(3).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).headPosition.position.channelId, 2);
    // ASSERT_NEAR(result.states.at(4).dropletPositions.at(droplet0).headPosition.position.position, 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).headPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).tailPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(4).dropletPositions.at(droplet0).tailPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(4).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).headPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(5).dropletPositions.at(droplet0).headPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).headPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).tailPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(5).dropletPositions.at(droplet0).tailPosition.position.position, 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(5).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).headPosition.position.channelId, 4);
    // ASSERT_NEAR(result.states.at(6).dropletPositions.at(droplet0).headPosition.position.position, 0.850000, 5e-7);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).headPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).tailPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(6).dropletPositions.at(droplet0).tailPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(6).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).headPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(7).dropletPositions.at(droplet0).headPosition.position.position, 0.000000, 5e-7);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).headPosition.volumeTowards0, 0);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).tailPosition.position.channelId, 6);
    // ASSERT_NEAR(result.states.at(7).dropletPositions.at(droplet0).tailPosition.position.position, 0.150000, 5e-7);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).tailPosition.volumeTowards0, 1);
    // ASSERT_EQ(result.states.at(7).dropletPositions.at(droplet0).channelIds, std::vector<int>{});

    ASSERT_EQ(result.continuousPhaseId, fluid0);

    ASSERT_EQ(result.maximalAdaptiveTimeStep, 0);

    ASSERT_EQ(result.resistanceModel, 0);
}

TEST(Results, noSink1) {
    droplet::Simulator sim;

    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;

    auto flowRate = 3e-11;
    auto pump = sim.addFlowRatePump(nodeGroundId, node0Id, flowRate);

    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    auto c1 = sim.addChannel(node0Id, node1Id, cHeight, cWidth, cLength);
    auto c2 = sim.addChannel(node1Id, nodeGroundId, cHeight, cWidth, cLength);

    sim.addGround(nodeGroundId);

    auto fluid0 = sim.addFluid(1e-3, 1e3);
    auto fluid1 = sim.addFluid(3e-3, 1e3);
    sim.setContinuousPhase(fluid0);

    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    sim.checkChipValidity();

    droplet::SimulationResult result = sim.simulate();

    ASSERT_EQ(result.chip.name, "");
}

TEST(Results, noSink2) {
    droplet::Simulator sim;

    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;

    auto flowRate = 3e-11;
    auto pump = sim.addFlowRatePump(nodeGroundId, node0Id, flowRate);

    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    auto c1 = sim.addChannel(node0Id, node1Id, cHeight, cWidth, cLength);
    auto c2 = sim.addChannel(node1Id, nodeGroundId, cHeight, cWidth, cLength);

    sim.addGround(nodeGroundId);

    auto fluid0 = sim.addFluid(1e-3, 1e3);
    auto fluid1 = sim.addFluid(3e-3, 1e3);
    sim.setContinuousPhase(fluid0);

    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.1, c1, 0.5);

    sim.checkChipValidity();

    droplet::SimulationResult result = sim.simulate();

    ASSERT_EQ(result.chip.name, "");
}

TEST(Results, noSinkTwoDroplets) {
    droplet::Simulator sim;

    int nodeGroundId = -1;
    int node0Id = 0;
    int node1Id = 1;

    auto flowRate = 3e-11;
    auto pump = sim.addFlowRatePump(nodeGroundId, node0Id, flowRate);

    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    auto c1 = sim.addChannel(node0Id, node1Id, cHeight, cWidth, cLength);
    auto c2 = sim.addChannel(node1Id, nodeGroundId, cHeight, cWidth, cLength);

    sim.addGround(nodeGroundId);

    auto fluid0 = sim.addFluid(1e-3, 1e3);
    auto fluid1 = sim.addFluid(3e-3, 1e3);
    sim.setContinuousPhase(fluid0);

    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c2, 0.5);
    auto droplet1 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    sim.checkChipValidity();

    droplet::SimulationResult result = sim.simulate();

    ASSERT_EQ(result.chip.name, "");
}
