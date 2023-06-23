#include "Fluid.h"

#include <string>
#include <utility>

namespace sim {

Fluid::Fluid(int id, double viscosity, double density, double concentration) : id(id), viscosity(viscosity), density(density), concentration(concentration) {}

void Fluid::setName(std::string name) {
    this->name = std::move(name);
}

int Fluid::getId() const {
    return id;
}

std::string Fluid::getName() const {
    return name;
}

double Fluid::getViscosity() const {
    return viscosity;
}

double Fluid::getDensity() const {
    return density;
}

double Fluid::getConcentration() const {
    return concentration;
}

void Fluid::addMixedFluid(Fluid* fluid) {
    mixedFluids.push_back(fluid);
}

const std::vector<Fluid*>& Fluid::getMixedFluids() const {
    return mixedFluids;
}

}  // namespace sim