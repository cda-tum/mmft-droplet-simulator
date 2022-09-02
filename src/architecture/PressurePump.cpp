#include "PressurePump.h"

#include "Edge.h"
#include "Node.h"

namespace arch {

PressurePump::PressurePump(int id, Node* node0, Node* node1, double pressure) : Edge(id, node0, node1), pressure(pressure) {}

void PressurePump::setPressure(double pressure) {
    this->pressure = pressure;
}

void PressurePump::setFlowRate(double flowRate) {
    this->flowRate = flowRate;
}

double PressurePump::getPressure() const {
    return pressure;
}

double PressurePump::getFlowRate() const {
    return flowRate;
}

double PressurePump::getResistance() const {
    return getPressure() / getFlowRate();
}

}  // namespace arch
