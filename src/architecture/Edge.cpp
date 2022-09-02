#include "Edge.h"

#include "IEdge.h"
#include "Node.h"

namespace arch {

Edge::Edge(int id, Node* node0, Node* node1) : id(id), node0(node0), node1(node1){};

void Edge::setName(std::string name) {
    this->name = std::move(name);
}

int Edge::getId() const {
    return id;
}

std::string Edge::getName() const {
    return name;
}

Node* Edge::getNode0() const {
    return node0;
}

Node* Edge::getNode1() const {
    return node1;
}

}  // namespace arch