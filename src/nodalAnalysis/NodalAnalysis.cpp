#include "NodalAnalysis.h"

#include <tuple>
#include <unordered_map>
#include <vector>

#include "Eigen/Dense"
#include "IFlowRatePump.h"
#include "INode.h"
#include "IPressurePump.h"
#include "IResistance.h"
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace nodal {

void conductNodalAnalysis(const std::unordered_map<int, std::tuple<INode*, int>>& nodes, const std::vector<IResistance*>& channels, const std::vector<IPressurePump*>& pressurePumps, const std::vector<IFlowRatePump*>& flowRatePumps) {
    const int nNodes = nodes.size() - 1;  // -1 due to ground node
    const int nPressurePumps = pressurePumps.size();
    const int nFlowRatePumps = flowRatePumps.size();
    const int nNodesAndPressurePumps = nNodes + nPressurePumps;
    const int groundNodeMatrixId = -1;

    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(nNodesAndPressurePumps, nNodesAndPressurePumps);  // matrix A = [G, B; C, D]
    Eigen::VectorXd z = Eigen::VectorXd::Zero(nNodesAndPressurePumps);                          // vector z = [i; e]

    // loop through resistances and build matrix G
    for (const auto channel : channels) {
        auto node0MatrixId = std::get<1>(nodes.at(channel->getNode0()->getId()));
        auto node1MatrixId = std::get<1>(nodes.at(channel->getNode1()->getId()));
        const double conductance = 1 / channel->getResistance();

        // main diagonal elements of G
        if (node0MatrixId != groundNodeMatrixId) {
            A(node0MatrixId, node0MatrixId) += conductance;
        }

        if (node1MatrixId != groundNodeMatrixId) {
            A(node1MatrixId, node1MatrixId) += conductance;
        }

        // minor diagonal elements of G (if no ground node was present)
        if (node0MatrixId != groundNodeMatrixId && node1MatrixId != groundNodeMatrixId) {
            A(node0MatrixId, node1MatrixId) -= conductance;
            A(node1MatrixId, node0MatrixId) -= conductance;
        }
    }

    // loop through pressurePumps and build matrix B, C and vector e
    int iPump = nNodes;
    for (const auto pressurePump : pressurePumps) {
        auto node0MatrixId = std::get<1>(nodes.at(pressurePump->getNode0()->getId()));
        auto node1MatrixId = std::get<1>(nodes.at(pressurePump->getNode1()->getId()));

        if (node0MatrixId != groundNodeMatrixId) {
            A(node0MatrixId, iPump) = -1;
            A(iPump, node0MatrixId) = -1;
        }
        if (node1MatrixId != groundNodeMatrixId) {
            A(node1MatrixId, iPump) = 1;
            A(iPump, node1MatrixId) = 1;
        }

        z(iPump) = pressurePump->getPressure();

        iPump++;
    }

    // loop through flowRatePumps and build vector i
    for (const auto flowRatePump : flowRatePumps) {
        auto node0MatrixId = std::get<1>(nodes.at(flowRatePump->getNode0()->getId()));
        auto node1MatrixId = std::get<1>(nodes.at(flowRatePump->getNode1()->getId()));
        const double flowRate = flowRatePump->getFlowRate();

        if (node0MatrixId != groundNodeMatrixId) {
            z(node0MatrixId) = -flowRate;
        }
        if (node1MatrixId != groundNodeMatrixId) {
            z(node1MatrixId) = flowRate;
        }
    }

    // solve equation x = Aˆ(-1) * z
    VectorXd x = A.colPivHouseholderQr().solve(z);

    // set pressure of nodes to result value
    for (const auto& [key, value] : nodes) {
        auto node = std::get<0>(value);
        auto nodeMatrixId = std::get<1>(value);
        if (nodeMatrixId != groundNodeMatrixId) {
            node->setPressure(x(nodeMatrixId));
        } else {
            node->setPressure(0.0);
        }
    }

    // set flow rate at pressure pumps
    iPump = nNodes;
    for (const auto pressurePump : pressurePumps) {
        pressurePump->setFlowRate(x(iPump));
        iPump++;
    }
}

}  // namespace nodal