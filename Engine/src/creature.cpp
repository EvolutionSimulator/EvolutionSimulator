#include "creature.h"

Creature::Creature()
    : MovableEntity()
{

}

double Creature::GetEnergy() const {
    return energy_;
}


void Creature::SetEnergy(double energy) {
    energy_ = energy;
}
