#include <iostream>

#include "droplet-simulator/Results.h"
#include "droplet-simulator/Simulator.h"

int main(int argc, char const* argv[]) {
    // 1. create the simulator
    std::cout << "--- Main ---" << std::endl;
    droplet::Simulator sim;

    // 2. add pumps
    std::cout << "--- flowRatePump ---" << std::endl;
    auto flowRate = 3e-11;
    // create flow-rate pump from node 0 to node 1 with the given flow-rate
    auto pump = sim.addFlowRatePump(0, 1, flowRate);

    // 3. add channels
    std::cout << "--- channels ---" << std::endl;
    auto cWidth = 100e-6;
    auto cHeight = 30e-6;
    auto cLength = 1000e-6;

    // create channel from node 1 to node 2 with the given height, width, length
    auto c1 = sim.addChannel(1, 2, cHeight, cWidth, cLength);
    // create channel from node 2 to node 3 with the given height, width, length
    auto c2 = sim.addChannel(2, 3, cHeight, cWidth, cLength);
    // create channel from node 2 to node 4 with the given height, width, length
    auto c3 = sim.addChannel(2, 4, cHeight, cWidth, 0.5 * cLength);
    // create channel from node 4 to node 5 with the given height, width, length
    auto c4 = sim.addChannel(4, 5, cHeight, cWidth, cLength);
    // create channel from node 5 to node 3 with the given height, width, length
    auto c5 = sim.addChannel(5, 3, cHeight, cWidth, 0.5 * cLength);
    // create channel from node 3 to node 0 with the given height, width, length
    auto c6 = sim.addChannel(3, 0, cHeight, cWidth, cLength);

    // 4. define sinks
    std::cout << "--- sink ---" << std::endl;
    // define that node 0 is a sink
    sim.addSink(0);

    // 5. define ground node
    std::cout << "--- ground ---" << std::endl;
    // define that node 0 is the ground node
    sim.addGround(0);
    sim.addGround(3);

    // 6. add fluids
    std::cout << "--- fluids ---" << std::endl;
    // add fluid with 1e-3 viscosity and 1e3 density
    auto fluid0 = sim.addFluid(1e-3, 1e3);
    // add fluid with 3e-3 viscosity and 1e3 density
    auto fluid1 = sim.addFluid(3e-3, 1e3);

    // 7. define the continuous phase
    std::cout << "--- continuousPhase ---" << std::endl;
    sim.setContinuousPhase(fluid0);

    // 8. add droplets
    std::cout << "--- droplet ---" << std::endl;
    auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    // create a droplet of fluid1, with a given droplet volume, injected at injectionTime 0.0
    // in channel with the channelId c1 at relative injection position 0.5
    auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    // 9. (optional) check if chip is valid
    std::cout << "--- validity check chip ---" << std::endl;
    sim.checkChipValidity();

    // 10. run the simulation
    std::cout << "--- simulate ---" << std::endl;
    auto result = sim.simulate();

    // print the results
    std::cout << "--- result ---" << std::endl;
    std::cout << result.toJson(4) << std::endl;

    return 0;
}
