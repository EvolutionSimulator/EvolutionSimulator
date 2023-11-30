#include "movable_entity.h".h"
#include "math.h"

MovableEntity::MovableEntity()
    : Entity(), velocity_forward_(0.0), rotational_velocity_(0.0) {
}

double MovableEntity::GetVelocityForward() const {
    return velocity_forward_;
}

double MovableEntity::GetRotationalVelocity() const {
    return rotational_velocity_;
}

void MovableEntity::SetVelocityForward(double velocity) {
    velocity_forward_ = velocity;
}

void MovableEntity::SetRotationalVelocity(double rotational_velocity) {
    rotational_velocity_ = rotational_velocity;
}

void MovableEntity::Rotate(double deltaTime)
{
    this->SetOrientation(this->GetOrientation() + deltaTime * this->GetRotationalVelocity());
}

void MovableEntity::Move(double deltaTime)
{
    std::pair<double, double> coordinates = this->GetCoordinates();
    double orientation = this->GetOrientation();
    double x_velocity = this->GetVelocityForward() * sin(orientation);
    double y_velocity = this->GetVelocityForward() * cos(orientation);
    coordinates.first = coordinates.first + deltaTime * x_velocity;
    coordinates.second = coordinates.second + deltaTime * y_velocity;
    this->SetCoordinates(coordinates.first, coordinates.second);
}
