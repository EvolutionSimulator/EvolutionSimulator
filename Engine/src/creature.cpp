#include "creature.h"


Creature::Creature()
    : MovableEntity(), energy_(0.0) {
}

double Creature::GetEnergy() const {
    return energy_;
}

void Creature::SetEnergy(double energy) {
    energy_ = energy;
}
