#include "FlowRatePump.h"

#include "Edge.h"
#include "Node.h"

namespace arch {

FlowRatePump::FlowRatePump(int id, Node* node0, Node* node1, double flowRate) : Edge(id, node0, node1), flowRate(flowRate) {}

void FlowRatePump::setFlowRate(double flowRate) {
    this->flowRate = flowRate;
}

double FlowRatePump::getPressure() const {
    return node0->getPressure() - node1->getPressure();
}

double FlowRatePump::getFlowRate() const {
    return flowRate;
}

double FlowRatePump::getResistance() const {
    return getPressure() / getFlowRate();
}

}  // namespace arch
