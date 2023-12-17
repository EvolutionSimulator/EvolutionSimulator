#include "movable_entity.h"
#include "config.h"
#include "math.h"

MovableEntity::MovableEntity()
    : Entity(), acceleration_(0), acceleration_angle(0),
      rotational_acceleration(0), velocity_(0), velocity_angle_(0),
      rotational_velocity_(0), strafing_difficulty(0.5) {}

double MovableEntity::GetAcceleration() const { return acceleration_; }

double MovableEntity::GetAccelerationAngle() const {
  return acceleration_angle;
}

double MovableEntity::GetRotationalAcceleration() const {
  return rotational_acceleration;
}

double MovableEntity::GetVelocity() const { return velocity_; }

double MovableEntity::GetVelocityAngle() const { return velocity_angle_; }

double MovableEntity::GetRotationalVelocity() const {
  return rotational_velocity_;
}

void MovableEntity::SetAcceleration(double acceleration) {
  acceleration_ = acceleration;
}

void MovableEntity::SetAccelerationAngle(double angle) {
  acceleration_angle = angle;
}

void MovableEntity::SetRotationalAcceleration(double rotational_acceleration) {
  rotational_acceleration = rotational_acceleration;
}

void MovableEntity::SetVelocity(double velocity) { velocity_ = velocity; }

void MovableEntity::SetVelocityAngle(double angle) { velocity_angle_ = angle; }

void MovableEntity::SetRotationalVelocity(double rotational_velocity) {
  rotational_velocity_ = rotational_velocity;
}

double MovableEntity::GetForwardFriction() const {
  return GetVelocity() * GetSize() *
         settings::environment::kFrictionalCoefficient *
         (1 + strafing_difficulty * abs(sin(GetVelocityAngle())));
}

double MovableEntity::GetEffectiveAccelerationAngle() const {
  // angle using the friction angle (dependent on velocity) and the acceleration
  double accel = GetAcceleration();
  double accel_angle = GetAccelerationAngle();
  double f = GetForwardFriction();
  double f_angle = GetVelocityAngle() + M_PI;
  return atan2(accel * sin(accel_angle) + f * sin(f_angle),
               accel * cos(accel_angle) + f * cos(f_angle));
}

double MovableEntity::GetEffectiveForwardAcceleration() const {
  double acceleration = GetAcceleration();
  double friction = GetForwardFriction();
  // return the acceleration magnitude using the cosine formula
  return sqrt(pow(acceleration, 2) + pow(friction, 2) +
              2 * acceleration * friction *
                  cos(acceleration_angle - GetVelocityAngle()));
}

double MovableEntity::GetRotationalFriction() const {
  return GetRotationalVelocity() * GetSize() *
         settings::environment::kFrictionalCoefficient;
}

double MovableEntity::GetEffectiveRotationalAcceleration() const {
  return rotational_acceleration - GetRotationalFriction();
}

void MovableEntity::UpdateVelocities(double deltaTime) {
  // Get absolute x and y components of velocity
  double velocity_x =
      GetVelocity() * cos(GetOrientation() + GetVelocityAngle());
  double velocity_y =
      GetVelocity() * sin(GetOrientation() + GetVelocityAngle());
  // update absolute x and y components of velocity
  velocity_x += GetEffectiveForwardAcceleration() *
                cos(GetEffectiveAccelerationAngle()) * deltaTime;
  velocity_y += GetEffectiveForwardAcceleration() *
                sin(GetEffectiveAccelerationAngle()) * deltaTime;
  // update velocity
  SetVelocity(sqrt(pow(velocity_x, 2) + pow(velocity_y, 2)));
  // update velocity angle
  SetVelocityAngle(atan2(velocity_y, velocity_x) - GetOrientation());
  // update rotational velocity
  SetRotationalVelocity(GetRotationalVelocity() +
                        GetEffectiveRotationalAcceleration() * deltaTime);
}

void MovableEntity::Move(double deltaTime, const double kMapWidth,
                         const double kMapHeight) {
  double orientation_rad = GetOrientation();

  double delta_x =
      velocity_ * cos(orientation_rad + velocity_angle_) * deltaTime;
  double delta_y =
      velocity_ * sin(orientation_rad + velocity_angle_) * deltaTime;

  // Update position
  auto [current_x, current_y] = GetCoordinates();
  double new_x = current_x + delta_x;

  double new_y = std::fmod(current_y + delta_y, kMapHeight);

  SetCoordinates(new_x, new_y, kMapWidth, kMapHeight);
}

void MovableEntity::Rotate(double deltaTime) {
  double new_orientation =
      GetOrientation() + (GetRotationalVelocity() * deltaTime);
  SetOrientation(new_orientation);
}

void MovableEntity::OnCollision(Entity &other_entity, double const kMapWidth,
                                double const kMapHeight) {
  Entity::OnCollision(other_entity, kMapWidth, kMapHeight);
}
