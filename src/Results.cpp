#include "Results.h"

#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

namespace droplet {

ChannelPosition::ChannelPosition(int channelId, double position) : channelId(channelId), position(position) {}

DropletBoundary::DropletBoundary(int channelId, double position, bool volumeTowards0, double flowRate, BoundaryState state) : position(ChannelPosition(channelId, position)), volumeTowards0(volumeTowards0), flowRate(flowRate), state(state) {}

DropletPosition::DropletPosition(DropletState state) : state(state) {}

Fluid::Fluid(int id, std::string name, double viscosity, double density, double concentration) : id(id), name(name), viscosity(viscosity), density(density), concentration(concentration) {}

Droplet::Droplet(int id, std::string name, double volume, int fluidId) : id(id), name(name), volume(volume), fluidId(fluidId) {}

Injection::Injection(int id, int dropletId, double time, int channelId, double position) : id(id), dropletId(dropletId), time(time), position(channelId, position) {}

Channel::Channel(int id, std::string name, int node0Id, int node1Id, double width, double height, double length, ChannelType type) : id(id), name(name), node0Id(node0Id), node1Id(node1Id), width(width), height(height), length(length), type(type) {}

FlowRatePump::FlowRatePump(int id, std::string name, int node0Id, int node1Id, double flowRate) : id(id), name(name), node0Id(node0Id), node1Id(node1Id), flowRate(flowRate) {}

PressurePump::PressurePump(int id, std::string name, int node0Id, int node1Id, double pressure) : id(id), name(name), node0Id(node0Id), node1Id(node1Id), pressure(pressure) {}

State::State(int id, double time) : id(id), time(time) {}

double State::getPressure(int nodeId) const {
    return pressures.at(nodeId);
}

double State::getPressureDrop(int node0Id, int node1Id) const {
    return getPressure(node0Id) - getPressure(node1Id);
}

double State::getFlowRate(int channelId) const {
    return flowRates.at(channelId);
}

DropletPathPosition::DropletPathPosition(int stateId) : stateId(stateId) {}

DropletPath::DropletPath(int dropletId) : dropletId(dropletId) {}

std::string DropletPath::toJson(int indent) const {
    nlohmann::json json;

    json["dropletId"] = dropletId;
    json["positions"] = nlohmann::json::array();
    for (auto& position : positions) {
        //channelIds
        auto channelIds = nlohmann::json::array();
        for (auto channelId : position.channelIds) {
            channelIds.push_back(channelId);
        }

        json["positions"].push_back({{"stateId", position.stateId}, {"channelIds", channelIds}});
    }

    return json.dump(indent);
}

DropletPath SimulationResult::getDropletPath(int dropletId) const {
    DropletPath dropletPath(dropletId);

    //loop through states
    for (auto const& state : states) {
        //get dropletPosition
        auto itDropletPosition = state.dropletPositions.find(dropletId);
        if (itDropletPosition != state.dropletPositions.end()) {
            //only consider droplets that are inside the Network
            if (itDropletPosition->second.state != DropletState::NETWORK) {
                continue;
            }

            //create DropletPathPosition
            auto& position = dropletPath.positions.emplace_back(state.id);

            //add channelIds of boundaries
            for (auto& boundary : itDropletPosition->second.boundaries) {
                position.channelIds.insert(boundary.position.channelId);
            }

            //add fully occupied channelIds
            for (auto& channelId : itDropletPosition->second.channelIds) {
                position.channelIds.insert(channelId);
            }

            //check if the set with the channelIds is the same as in the previous state
            //if yes then do not consider this DropletPathPosition and pop it from the list (prevents duplicates)
            if (dropletPath.positions.size() > 1 && dropletPath.positions[dropletPath.positions.size() - 2].channelIds == position.channelIds) {
                dropletPath.positions.pop_back();
            }
        }
    }

    return dropletPath;
}

std::string SimulationResult::toJson(int indent) const { 
    nlohmann::json json;

    json["results"] = nlohmann::json::object();
    json["results"]["network"] = nlohmann::json::array();
    for (auto const& state : states) {

        auto nodes = nlohmann::json::array();
        for (auto const& [key, pressure] : state.pressures) {
            nodes.push_back({{"pressure", pressure}});
        }

        auto channels = nlohmann::json::array();
        for (auto const& [key, flowRate] : state.flowRates) {
            channels.push_back({{"flowRate", flowRate}});
        }

        auto bigDroplets = nlohmann::json::array();
        for (auto const& [key, dropletPosition] : state.dropletPositions) {
            //dropletPosition
            auto bigDroplet = nlohmann::json::object();

            //state
            bigDroplet["id"] = key;
            bigDroplet["fluid"] = droplets.at(key).fluidId;

            //boundaries
            bigDroplet["boundaries"] = nlohmann::json::array();
            for(auto const &boundary : dropletPosition.boundaries) {
                bigDroplet["boundaries"].push_back({
                    {"volumeTowards1", boundary.volumeTowards0},
                    {"position", {
                        {"channelId", boundary.position.channelId},
                        {"position", boundary.position.position}}
                    }
                });
            }

            //channels
            bigDroplet["channels"] = nlohmann::json::array();
            for (auto const& channelId : dropletPosition.channelIds) {
                bigDroplet["channels"].push_back(channelId);
            }
            bigDroplets.push_back(bigDroplet);
        }

        json["network"].push_back({{"time", state.time}, {"nodes", nodes}, {"channels", channels}, {"bigDroplets", bigDroplets}});
    }
    return json.dump(indent);
}
/*
std::string SimulationResult::toJson(int indent) const {
    nlohmann::json json;

    json["continuousPhaseId"] = continuousPhaseId;
    json["maximalAdaptiveTimeStep"] = maximalAdaptiveTimeStep;
    json["resistanceModel"] = resistanceModel;

    // chip
    json["chip"] = nlohmann::json::object();
    json["chip"]["name"] = chip.name;
    json["chip"]["channels"] = nlohmann::json::array();
    for (auto const& [key, channel] : chip.channels) {
        json["chip"]["channels"].push_back({{"id", channel.id}, {"name", channel.name}, {"node0Id", channel.node0Id}, {"node1Id", channel.node1Id}, {"width", channel.width}, {"height", channel.height}, {"length", channel.length}, {"type", channel.type}});
    }
    json["chip"]["flowRatePumps"] = nlohmann::json::array();
    for (auto const& [key, flowRatePump] : chip.flowRatePumps) {
        json["chip"]["flowRatePumps"].push_back({{"id", flowRatePump.id}, {"name", flowRatePump.name}, {"node0Id", flowRatePump.node0Id}, {"node1Id", flowRatePump.node1Id}, {"flowRate", flowRatePump.flowRate}});
    }
    json["chip"]["pressurePumps"] = nlohmann::json::array();
    for (auto const& [key, pressurePump] : chip.pressurePumps) {
        json["chip"]["pressurePumps"].push_back({{"id", pressurePump.id}, {"name", pressurePump.name}, {"node0Id", pressurePump.node0Id}, {"node1Id", pressurePump.node1Id}, {"pressure", pressurePump.pressure}});
    }

    // fluids
    json["fluids"] = nlohmann::json::array();
    for (auto const& [key, fluid] : fluids) {
        json["fluids"].push_back({{"id", fluid.id}, {"name", fluid.name}, {"mixedFluidIds", fluid.mixedFluidIds}, {"viscosity", fluid.viscosity}, {"density", fluid.density}, {"concentration", fluid.concentration}});
    }

    // droplets
    json["droplets"] = nlohmann::json::array();
    for (auto const& [key, droplet] : droplets) {
        json["droplets"].push_back({{"id", droplet.id}, {"name", droplet.name}, {"mergedDropletIds", droplet.mergedDropletIds}, {"volume", droplet.volume}, {"fluidId", droplet.fluidId}});
    }

    // injections
    json["injections"] = nlohmann::json::array();
    for (auto const& [key, injection] : injections) {
        json["injections"].push_back({{"id", injection.id}, {"dropletId", injection.dropletId}, {"time", injection.time}, {"position", {{"channelId", injection.position.channelId}, {"position", injection.position.position}}}});
    }

    // states
    json["states"] = nlohmann::json::array();
    for (auto const& state : states) {
        //pressures
        auto pressures = nlohmann::json::object();
        for (auto const& [key, pressure] : state.pressures) {
            pressures[std::to_string(key)] = pressure;
        }

        //flowRates
        auto flowRates = nlohmann::json::object();
        for (auto const& [key, flowRate] : state.flowRates) {
            flowRates[std::to_string(key)] = flowRate;
        }

        //dropletPositions
        auto dropletPositions = nlohmann::json::object();
        for (auto const& [key, dropletPosition] : state.dropletPositions) {
            //dropletPosition
            auto jsonDropletPosition = nlohmann::json::object();

            //state
            jsonDropletPosition["state"] = dropletPosition.state;

            //boundaries
            // clang-format off
            jsonDropletPosition["boundaries"] = nlohmann::json::array();
            for(auto const &boundary : dropletPosition.boundaries) {
                jsonDropletPosition["boundaries"].push_back({
                    {"volumeTowards1", boundary.volumeTowards0},
                    {"position", {
                        {"channelId", boundary.position.channelId},
                        {"position", boundary.position.position}}
                    }
                });
            }
            // clang-format on

            //channelIds
            jsonDropletPosition["channelIds"] = nlohmann::json::array();
            for (auto const& channelId : dropletPosition.channelIds) {
                jsonDropletPosition["channelIds"].push_back(channelId);
            }

            //add dropletPosition to array
            dropletPositions[std::to_string(key)] = jsonDropletPosition;
        }

        //add state to array
        json["states"].push_back({{"id", state.id}, {"time", state.time}, {"pressures", pressures}, {"flowRates", flowRates}, {"dropletPositions", dropletPositions}});
    }

    return json.dump(indent);
}
*/
SimulationResult SimulationResult::fromJson(std::string jsonString) {
    auto json = nlohmann::json::parse(jsonString);

    SimulationResult results;

    results.continuousPhaseId = json["continuousPhaseId"];
    results.maximalAdaptiveTimeStep = json["maximalAdaptiveTimeStep"];
    results.resistanceModel = json["resistanceModel"];

    //###chip###
    //name
    results.chip.name = json["chip"]["name"];

    //channels
    for (auto& channel : json["chip"]["channels"]) {
        results.chip.channels.try_emplace(channel["id"], channel["id"], channel["name"], channel["node0Id"], channel["node1Id"], channel["width"], channel["height"], channel["length"], static_cast<ChannelType>(channel["type"]));
    }

    //flowRatePumps
    for (auto& pump : json["chip"]["flowRatePumps"]) {
        results.chip.flowRatePumps.try_emplace(pump["id"], pump["id"], pump["name"], pump["node0Id"], pump["node1Id"], pump["flowRate"]);
    }

    //pressurePumps
    for (auto& pump : json["chip"]["pressurePumps"]) {
        results.chip.pressurePumps.try_emplace(pump["id"], pump["id"], pump["name"], pump["node0Id"], pump["node1Id"], pump["pressure"]);
    }

    //##fluids###
    for (auto& fluid : json["fluids"]) {
        auto [value, success] = results.fluids.try_emplace(fluid["id"], fluid["id"], fluid["name"], fluid["viscosity"], fluid["density"], fluid["concentration"]);
        for (auto& fluidId : fluid["mixedFluidIds"]) {
            value->second.mixedFluidIds.emplace_back(fluidId);
        }
    }

    //##droplets###
    for (auto& droplet : json["droplets"]) {
        auto [value, success] = results.droplets.try_emplace(droplet["id"], droplet["id"], droplet["name"], droplet["volume"], droplet["fluidId"]);
        for (auto& dropletId : droplet["mergedDropletIds"]) {
            value->second.mergedDropletIds.emplace_back(dropletId);
        }
    }

    //##injections###
    for (auto& injection : json["injections"]) {
        results.injections.try_emplace(injection["id"], injection["id"], injection["dropletId"], injection["time"], injection["position"]["channelId"], injection["position"]["position"]);
    }

    //###states###
    for (auto& jsonState : json["states"]) {
        //create state (with corresponding id and time)
        auto& state = results.states.emplace_back(jsonState["id"], jsonState["time"]);

        //pressures
        for (auto& [key, pressure] : jsonState["pressures"].items()) {
            state.pressures.try_emplace(std::stoi(key), pressure);
        }

        //flowRates
        for (auto& [key, flowRate] : jsonState["flowRates"].items()) {
            state.flowRates.try_emplace(std::stoi(key), flowRate);
        }

        //dropletPositions
        for (auto& [key, jsonDropletPosition] : jsonState["dropletPositions"].items()) {
            //create dropletPosition
            auto [value, success] = state.dropletPositions.try_emplace(std::stoi(key), static_cast<DropletPosition>(jsonDropletPosition["state"]));

            //boundaries
            for (auto& boundary : jsonDropletPosition["boundaries"]) {
                value->second.boundaries.emplace_back(boundary["position"]["channelId"], boundary["position"]["position"], boundary["volumeTowards0"], boundary["flowRate"], static_cast<BoundaryState>(jsonDropletPosition["state"]));
            }

            //channelIds
            for (auto& channelId : jsonDropletPosition["channelIds"]) {
                value->second.channelIds.emplace_back(channelId);
            }
        }
    }

    return results;
}


std::unordered_map<int, double> SimulationResult::getPressures() const {
    return states[0].pressures;
}

std::unordered_map<int, double> SimulationResult::getFlowRates() const {
    return states[0].flowRates;
}

}  // namespace droplet
