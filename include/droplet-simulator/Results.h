/**
 * @file Results.h
 */

#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace droplet {

/**
 * @brief Struct to contain a channel position specified by channel id and position.
 */
struct ChannelPosition {
    int channelId;    ///< Id of the channel.
    double position;  ///< The relative position (between 0.0 and 1.0) within the channel.

    /**
     * @brief Construct a channel position.
     * @param[in] channelId Id of the channel.
     * @param[in] position Relative position inside the channel.
     */
    ChannelPosition(int channelId, double position);
};

/**
 * @brief Enum to specify in which state a droplet boundary is currently in.
 */
enum class BoundaryState { NORMAL, WAIT_INFLOW, WAIT_OUTFLOW };

/**
 * @brief Struct containing a boundary of a droplet.
 */
struct DropletBoundary {
    ChannelPosition position;  ///< At which channel and at which position within the channel the droplet boundary is located.
    bool volumeTowards0;       ///< Indicates if the volume of the droplet is located from the current position towards node 0 (or if false towards node 1).
    double flowRate;           ///< Current flow rate of the boundary
    BoundaryState state;       ///< State in which the boundary is currently in

    /**
     * @brief Construct a droplet boundary
     * @param[in] channelId Id of the channel the boundary is present.
     * @param[in] position Relative position of the boundary within the channel.
     * @param[in] volumeTowards0 Direction in which the volume of the boundary is located (true if it is towards node0).
     * @param[in] flowRate Current flow rate of the boundary.
     * @param[in] state State in which the boundary is currently in.
     */
    DropletBoundary(int channelId, double position, bool volumeTowards0, double flowRate, BoundaryState state);
};

/**
 * @brief Enum to specify in which state the droplet is currently in.
 */
enum class DropletState {
    INJECTION,  ///< Droplet planned to be injected but currently not yet in the network.
    NETWORK,    ///< Droplet currently flows through the network.
    TRAPPED,    ///< Droplet is trapped in the network.
    SINK        ///< Droplet has left the network (is in the sink).
};

/**
 * @brief Struct to contain the current position of a droplet during a particular time step.
 */
struct DropletPosition {
    std::vector<DropletBoundary> boundaries;  ///< Contain all boundaries which are present during a particular time step.
    std::vector<int> channelIds;              ///< Contains the ids of the channels that are fully occupied by the droplet.
    DropletState state;                       ///< State in which the droplet is currently in.

    /**
     * @brief Constructs a droplet position.
     * @param[in] state State in which the droplet is currently in.
     */
    DropletPosition(DropletState state);
};

/**
 * @brief Struct to contain the fluid specified by id, name, viscosity, density and concentration.
 */
struct Fluid {
    int id;                          ///< Id of the fluid.
    std::string name;                ///< Name of the fluid.
    std::vector<int> mixedFluidIds;  ///< List of previous fluid ids, if this fluid was generated by mixing.
    double viscosity;                ///< Viscosity of the fluid in Pa s.
    double density;                  ///< Density of the fluid in kg/m^3.
    double concentration;            ///< Concentration of the fluid in % (between 0.0 and 1.0).

    /**
     * @brief Constructs a fluid.
     * @param[in] id Unique identifier of the fluid.
     * @param[in] name Name of the channel.
     * @param[in] viscosity Viscosity of the fluid in Pas.
     * @param[in] density Density of the fluid in kg/m^3.
     * @param[in] concentration Concentration of the fluid in % (between 0.0 and 1.0).
     */
    Fluid(int id, std::string name, double viscosity, double density, double concentration);
};

/**
 * @brief Struct to contain a droplet specified by the id, name, volume, fluid, splitId and mergeIds.
 */
struct Droplet {
    int id;                             ///< Id of the droplet.
    std::string name;                   ///< Name of the droplet.
    std::vector<int> mergedDropletIds;  ///< List of previous droplet ids, if this droplet got merged.
    double volume;                      ///< Volume of the droplet in m^3.
    int fluidId;                        ///< Fluid of the droplet.

    /**
     * @brief Constructs a droplet.
     * @param[in] id Unique identifier of the droplet.
     * @param[in] name Name of the channel.
     * @param[in] volume Volume of the droplet in m^3.
     * @param[in] fluidId Id of the fluid the droplet consists of.
     */
    Droplet(int id, std::string name, double volume, int fluidId);
};

/**
 * @brief Struct to contain an injection specified by id, dropletId, time, and a channel position.
 */
struct Injection {
    int id;                    ///< Id of the injection.
    int dropletId;             ///< Id of the droplet to be injected.
    double time;               ///< Time in s at which the injection takes place.
    ChannelPosition position;  ///< Position at which the droplet should be injected.

    /**
     * @brief Constructs an injection.
     * @param[in] id Unique identifier of an injection.
     * @param[in] dropletId Id of the droplet which gets injected.
     * @param[in] time Time at which the droplet is injected.
     * @param[in] channelId Id of the channel in which the droplet is injected.
     * @param[in] position Relative position (between 0.0 and 1.0) of the middle of the droplet inside channel.
     */
    Injection(int id, int dropletId, double time, int channelId, double position);
};

/**
 * @brief Enum to specify the channel type as normal, bypass or cloggable.
 */
enum class ChannelType {
    NORMAL,    ///< A normal channel is the regular channel in which droplets can flow.
    BYPASS,    ///< A bypass channel allows droplets to bypass another channel e.g. if a droplet is trapped in that channel.
    CLOGGABLE  ///< A cloggable channel will be clogged during the time a droplet passes by one of its ends.
};

/**
 * @brief Struct to contain a channel specified by id, name, two nodes, width, height, length and channel type.
 */
struct Channel {
    int id;            ///< Id of the channel.
    std::string name;  ///< Name of the channel.
    int node0Id;       ///< Id of node at one end of the channel.
    int node1Id;       ///< Id of node at the other end of the channel.
    double width;      ///< Width of the channel in m.
    double height;     ///< Height of the channel in m.
    double length;     ///< Length of the channel in m.
    ChannelType type;  ///< What kind of channel it is.

    /**
     * @brief Constructs a channel.
     * @param[in] id Id of the channel.
     * @param[in] name Name of the channel.
     * @param[in] node0Id Id of the node at the start of the channel.
     * @param[in] node1Id Id of the node at the end of the channel.
     * @param[in] width Width of the channel in m.
     * @param[in] height Height of the channel in m.
     * @param[in] length Length of the channel in m.
     * @param[in] type Type of the channel.
     */
    Channel(int id, std::string name, int node0Id, int node1Id, double width, double height, double length, ChannelType type);
};

/**
 * @brief Struct to contain a flow rate pump specified by id, name, two nodes and the flow rate.
 */
struct FlowRatePump {
    int id;            ///< Id of the flow rate pump.
    std::string name;  ///< Name of the flow rate pump.
    int node0Id;       ///< Id of the node at one end of the flow rate pump.
    int node1Id;       ///< Id of the node at the other end of the flow rate pump.
    double flowRate;   ///< Flow rate in m^3/s.

    /**
     * @brief Constructs a flow rate pump.
     * @param[in] id Id of the flow rate pump.
     * @param[in] name Name of the pump.
     * @param[in] node0Id Id of the node at the start of the pump.
     * @param[in] node1Id Id of the node at the end of the pump.
     * @param[in] flowRate Volumetric flow rate of the pump in m^3/s.
     */
    FlowRatePump(int id, std::string name, int node0Id, int node1Id, double flowRate);
};

/**
 * @brief Struct to contain a pressure pump specified by id, name, two nodes and the pressure.
 */
struct PressurePump {
    int id;            ///< Id of the pressure pump.
    std::string name;  ///< Name of the pressure pump.
    int node0Id;       ///< Id of the node at one end of the pressure pump.
    int node1Id;       ///< Id of the node at the other end of the pressure pump.
    double pressure;   ///< Pressure in Pa.

    /**
     * @brief Constructs a pressure pump.
     * @param[in] id Id of the pressure pump.
     * @param[in] name Name of the pump.
     * @param[in] node0Id Id of the node at the start of the pump.
     * @param[in] node1Id Id of the node at the end of the pump.
     * @param[in] pressure Pressure of the pump in Pa.
     */
    PressurePump(int id, std::string name, int node0Id, int node1Id, double pressure);
};

/**
 * @brief Struct to contain a chip specified by name, an unordered map of channels, an unordered map of flow rate pumps and an unordered map of pressure pumps.
 */
struct Chip {
    std::string name;                                     ///< Name of the chip.
    std::unordered_map<int, Channel> channels;            ///< Map that contains all channels of the network.
    std::unordered_map<int, FlowRatePump> flowRatePumps;  ///< Map that contains all flow rate pumps of the network.
    std::unordered_map<int, PressurePump> pressurePumps;  ///< Map that contains all pressure pumps of the network.
};

/**
 * @brief Struct to contain a state specified by time, an unordered map of pressures, an unordered map of flow rates, a vector of clogged channel ids, an unordered map of droplet positions.
 */
struct State {
    int id;                                                     ///< Sequential id of the state
    double time;                                                ///< Simulation time for which the following values were calculated.
    std::unordered_map<int, double> pressures;                  ///< Keys are the nodeIds.
    std::unordered_map<int, double> flowRates;                  ///< Keys are the edgeIds (channels and pumps).
    std::unordered_map<int, DropletPosition> dropletPositions;  ///< Only contains the position of droplets that are currently inside the network (key is the droplet id).
    
    /**
     * @brief Constructs a state, which represent a time step during a simulation.
     * @param[in] id Id of the state
     * @param[in] time Value of the current time step.
     */
    State(int id, double time);

    /**
     * @brief Function to get pressure at a specific node.
     * @param[in] nodeId Id of the node.
     * @return Pressure at this node in Pa.
     */
    double getPressure(int nodeId) const;

    /**
     * @brief Function to get pressure drop between two nodes.
     * @param[in] node0Id Id of one node.
     * @param[in] node1Id Id of the other node.
     * @return Pressure drop between these two nodes in Pa.
     */
    double getPressureDrop(int node0Id, int node1Id) const;

    /**
     * @brief Function to get flow rate at a specific channel.
     * @param[in] channelId Id of the channel.
     * @return Flowrate at this channel in m^3/s.
     */
    double getFlowRate(int channelId) const;
};

/**
 * @brief Struct that represents the location of a droplet at a specific state.
 It is utilized when computing the path of a droplet and only stores the channel ids of the boundaries and fully occupied channels (in comparison to a DropletPosition, which stores much more parameters).
 */
struct DropletPathPosition {
    int stateId;               ///< Id of the state
    std::set<int> channelIds;  ///< Set of channel ids that represent the actual location of all boundaries and fully occupied channels
    
    /**
     * @brief Constructs a droplet position for the DropletPath class.
     * @param[in] stateId Id of the state for which the class is used for.
     */
    DropletPathPosition(int stateId);
};

/**
 * @brief Struct that represents the path of a droplet during a simulation.
 */
struct DropletPath {
    int dropletId;                               ///< Id of the droplet
    std::vector<DropletPathPosition> positions;  ///< For each state a DropletPathPosition is stored, except for states where the droplet would have the same location as the previous one.
    
    /**
     * @brief Constructs a droplet path for a certain droplet.
     * @param[in] dropletId Id of the droplet.
     */
    DropletPath(int dropletId);

    /**
     * @brief Converts the struct to a json string.
     * @param[in] indent Indentation to use when the string is constructed. Default is -1, which disables the indentation.
     * @return json string
     */
    std::string toJson(int indent = -1) const;
};

/**
 * @brief Struct to contain the simulation result specified by a chip, an unordered map of fluids, an unordered map of droplets, an unordered map of injections, a vector of states, a continuous fluid id, the maximal adaptive time step and the id of a resistance model.
 */
struct SimulationResult {
    /// Contains the chip, with all the channels and pumps.
    Chip chip;

    /// Contains all fluids which were defined (i.e., also the fluids which were created when droplets merged).
    /// (key is the fluid id)
    std::unordered_map<int, Fluid> fluids;

    /// Contains all droplets that occurred during the simulation not only the once that were injected (i.e., also merged and splitted droplets)/
    /// The actual position of the droplets during the simulation is then stored inside the states.
    /// (key is the droplet id)
    std::unordered_map<int, Droplet> droplets;

    /// Contains all injections that happened during the simulation.
    /// (key is the injection id)
    std::unordered_map<int, Injection> injections;

    /// Contains all states ordered according to their simulation time (beginning at the start of the simulation).
    std::vector<State> states;

    /// Fluid id which served as the continuous phase.
    int continuousPhaseId;

    /// Value for the maximal adaptive time step that was used.
    double maximalAdaptiveTimeStep;

    /// Id of the used resistance model.
    int resistanceModel;

    /**
     * @brief Get the simulated path of a droplet.
     * @param dropletId Id of the droplet for which the path should be returned.
     * @return std::vector<std::pair<double, DropletPosition>> Every entry in the vector represents a simulated timepoint (first value) and the droplet position of the specified droplet at that time.
     */
    DropletPath getDropletPath(int dropletId) const;

    /**
     * @brief Converts the struct to a json string.
     * @param indent Indentation to use when the string is constructed. Default is -1, which disables the indentation.
     * @return json string
     */
    std::string toJson(int indent = -1) const;

    /**
     * @brief Reconstructs the struct from a json string.
     * @param json json string
     * @return SimulationResult struct
     */
    static SimulationResult fromJson(std::string json);
};

}  // namespace droplet