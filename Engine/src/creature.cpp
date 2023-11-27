#include "creature.h"

Creature::Creature()
    : Entity(), energy_(0.0), velocity_forward_(0.0), rotational_velocity_(0.0) {
}

double Creature::GetEnergy() const {
    return energy_;
}

double Creature::GetVelocityForward() const {
    return velocity_forward_;
}

double Creature::GetRotationalVelocity() const {
    return rotational_velocity_;
}

void Creature::SetEnergy(double energy) {
    energy_ = energy;
}

void Creature::SetVelocityForward(double velocity) {
    velocity_forward_ = velocity;
}

void Creature::SetRotationalVelocity(double rotational_velocity) {
    rotational_velocity_ = rotational_velocity;
}
