 \mainpage Modular Microfluidics Simulator Index Page
  \section intro_sec Introduction
    This is the API documentation of the 1D-Simulator for Microfluidic Biochips developed by the [Chair for Design Automation](https://www.cda.cit.tum.de/) at the [Technical University of Munich](https://www.tum.de/). 
    This 1D-Simulator exploits the 1D analysis model, which is especially suited for simulating designs before even the first prototype is fabricated and for design space explorations. Furthermore, it allows to simulate droplets and their respective paths inside a Lab-on-a-Chip (LoC) with closed micro-channels. 
    For more information about our work on Microfluidics, please visit https://www.cda.cit.tum.de/research/microfluidics/. 

    If you have any questions, feel free to contact us via microfluidics.cda@xcit.tum.de or by creating an issue on GitHub. 

  \section system_req System requirements
    The implementation should be compatible with any current C++ compiler supporting C++17 and a minimum CMake version 3.21. 
    If you have doxygen installed, a detailed documentation can be build with the following command inside the build folder of the project:
    \code{bash} make dropletDocumentation \endcode
    The documentation will be written to the doc folder within the build folder.

  \section use Usage
    To use this library include the following code in your cmake file:
    \code{cmake}
    include(FetchContent)
    FetchContent_Declare(
        droplet
        GIT_REPOSITORY https://github.com/cda-tum/mmft-droplet-simulator.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(droplet)

    target_link_libraries(${TARGET} PRIVATE droplet)
    \endcode
    and include the library API header in your project file:
    \code{.cpp}#include "droplet-simulator/Simulator.h"\endcode

  \section example Example
    This small example shows how to create and simulate a small network. 
    \code{cpp}
    #include <iostream>

    #include "droplet-simulator/Results.h"
    #include "droplet-simulator/Simulator.h"

    int main(int argc, char const* argv[]) {
        std::cout << "--- Main ---" << std::endl;
        // create the simulator
        droplet::Simulator sim;

        std::cout << "--- flowRatePump ---" << std::endl;
        // flowRate pump
        auto flowRate = 3e-11;
        // create flow-rate pump from node -1 to node 0 with the given flow-rate
        auto pump = sim.addFlowRatePump(-1, 0, flowRate);

        std::cout << "--- channels ---" << std::endl;
        // channels
        auto cWidth = 100e-6;
        auto cHeight = 30e-6;
        auto cLength = 1000e-6;

        // create channel from node 0 to node 1 with the given height, width, length
        auto c1 = sim.addChannel(0, 1, cHeight, cWidth, cLength);
        // create channel from node 1 to node 2 with the given height, width, length
        auto c2 = sim.addChannel(1, 2, cHeight, cWidth, cLength);
        // create channel from node 2 to node 3 with the given height, width, length
        auto c3 = sim.addChannel(2, 3, cHeight, cWidth, cLength);
        // create channel from node 2 to node 4 with the given height, width, length
        auto c4 = sim.addChannel(2, 4, cHeight, cWidth, cLength);
        // create channel from node 3 to node 4 with the given height, width, length
        auto c5 = sim.addChannel(3, 4, cHeight, cWidth, cLength);
        // create channel from node 4 to node -1 with the given height, width, length
        auto c6 = sim.addChannel(4, -1, cHeight, cWidth, cLength);
        std::cout << "--- sink ---" << std::endl;
        // define that node -1 is a sink
        sim.addSink(-1);
        std::cout << "--- ground ---" << std::endl;
        // define that node -1 is the ground node
        sim.addGround(-1);

        // fluids
        std::cout << "--- fluids ---" << std::endl;
        // add fluid with 1e-3 viscosity and 1e3 density
        auto fluid0 = sim.addFluid(1e-3, 1e3);
        // add fluid with 3e-3 viscosity and 1e3 density
        auto fluid1 = sim.addFluid(3e-3, 1e3);
        std::cout << "--- continuousPhase ---" << std::endl;
        // define which fluid is the continuous phase
        sim.setContinuousPhase(fluid0);

        // droplet
        std::cout << "--- droplet ---" << std::endl;
        auto dropletVolume = 1.5 * cWidth * cWidth * cHeight;
        // create a droplet of fluid1, with a given droplet volume, injected at injectionTime 0.0 in channel with the channelId c1 at relative injection position 0.5
        auto droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

        std::cout << "--- validity check chip ---" << std::endl;
        // check if chip is valid
        sim.checkChipValidity();

        std::cout << "--- simulate ---" << std::endl;
        // simulate the microfluidic network
        auto result = sim.simulate();

        // print the result
        std::cout << "--- result ---" << std::endl;
        std::cout << result.toJson(4) << std::endl;

        return 0;
    }
    \endcode

 
