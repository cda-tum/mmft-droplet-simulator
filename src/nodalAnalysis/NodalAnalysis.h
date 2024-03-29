/**
 * @file NodalAnalysis.h
 */
#ifndef BB2D29DC_BB06_4F08_9CF4_39AA7BEA52B8
#define BB2D29DC_BB06_4F08_9CF4_39AA7BEA52B8

#pragma once

#include <tuple>
#include <unordered_map>
#include <vector>

#include "INode.h"

namespace arch { class Channel; class FlowRatePump; class PressurePump; }
namespace nodal {

/**
 * @brief Conducts the Modifed Nodal Analysis (e.g., http://qucs.sourceforge.net/tech/node14.html) and computes the pressure levels for each node.
 * Hence, the passed nodes contain the final pressure levels when the function is finished.
 *
 * @param[in,out] nodes List of nodes, which contain the result (pressure values) of the modified nodal analysis.
 * @param[in] channels List of channels.
 * @param[in,out] pressurePumps List of pressure pumps.
 * @param[in] flowRatePumps List of flowrate pumps.
 */
void conductNodalAnalysis(const std::unordered_map<int, std::tuple<INode*, int>>& nodes, const std::vector<arch::Channel*>& channels, const std::vector<arch::PressurePump*>& pressurePumps, const std::vector<arch::FlowRatePump*>& flowRatePumps);

}  // namespace nodal

#endif
