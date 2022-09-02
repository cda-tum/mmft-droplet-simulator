#include "Node.h"

#include <memory>

namespace arch {

Node::Node(int id) : id(id) {}

void Node::setName(std::string name) {
    this->name = std::move(name);
}

void Node::setPressure(double pressure) {
    this->pressure = pressure;
}

std::string Node::getName() const {
    return name;
}

int Node::getId() const {
    return id;
}

double Node::getPressure() const {
    return pressure;
}

}  // namespace arch