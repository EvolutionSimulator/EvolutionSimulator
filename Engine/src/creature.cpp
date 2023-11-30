#include "creature.h"
#include "math.h"

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

void Creature::Rotate(double deltaTime)
{
    this->SetOrientation(this->GetOrientation() + deltaTime * this->GetRotationalVelocity());
}

void Creature::Move(double deltaTime)
{
    std::pair<double, double> coordinates = this->GetCoordinates();
    double orientation = this->GetOrientation();
    double x_velocity = this->GetVelocityForward() * sin(orientation);
    double y_velocity = this->GetVelocityForward() * cos(orientation);
    coordinates.first = coordinates.first + deltaTime * x_velocity;
    coordinates.second = coordinates.second + deltaTime * y_velocity;
    this->SetCoordinates(coordinates.first, coordinates.second);
}


void Creature::UpdateEnergy(){
    SetEnergy( GetEnergy() - (GetVelocityForward() + GetRotationalVelocity()) * GetSize() );

    if (GetEnergy() <= 0) {
        Dies();
    }
}

void Creature::Dies(){
    SetState(Dead);
}

void Creature::Eats(double nutritional_value){
    SetEnergy(GetEnergy() + nutritional_value);
}
