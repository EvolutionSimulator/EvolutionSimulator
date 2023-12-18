#include "movable_entity.h"

#include "config.h"
#include "math.h"

/*!
 * @file movable_entity.h
 * @brief Defines the MovableEntity class.
 *
 * The MovableEntity class extends from the Entity class, adding functionality
 * for motion including linear and rotational movements. It introduces motion
 * parameters such as acceleration, velocity, and rotational equivalents, along
 * with methods to manipulate and utilize these parameters. This class is
 * essential for simulating entities that move and interact in a dynamic
 * environment, providing the basis for more complex behaviors in derived
 * classes.
 */

/*!
 * @brief Constructs a new MovableEntity object.
 *
 * @details Initializes the movable entity with default motion parameters.
 */
MovableEntity::MovableEntity()
    : Entity(),
      acceleration_(0),
      acceleration_angle(0),
      rotational_acceleration(0),
      velocity_(0),
      velocity_angle_(0),
      rotational_velocity_(0),
      strafing_difficulty(0.5) {}

/*!
 * @brief Returns the linear acceleration.
 */
double MovableEntity::GetAcceleration() const { return acceleration_; }

/*!
 * @brief Returns the angle of acceleration.
 */
double MovableEntity::GetAccelerationAngle() const {
  return acceleration_angle;
}

/*!
 * @brief Returns the rotational acceleration.
 */
double MovableEntity::GetRotationalAcceleration() const {
  return rotational_acceleration;
}

/*!
 * @brief Returns the linear velocity.
 */
double MovableEntity::GetVelocity() const { return velocity_; }

/*!
 * @brief Returns the angle of velocity.
 */
double MovableEntity::GetVelocityAngle() const { return velocity_angle_; }

/*!
 * @brief Returns the rotational velocity.
 */
double MovableEntity::GetRotationalVelocity() const {
  return rotational_velocity_;
}

/*!
 * @brief Sets the linear acceleration of the entity.
 *
 * @param acceleration The new linear acceleration value.
 */
void MovableEntity::SetAcceleration(double acceleration) {
  acceleration_ = fabs(acceleration);
}

/*!
 * @brief Sets the angle of the linear acceleration.
 *
 * @param angle The new angle for the linear acceleration, measured in radians.
 */
void MovableEntity::SetAccelerationAngle(double angle) {
  acceleration_angle = angle;
}

/*!
 * @brief Sets the rotational acceleration of the entity.
 *
 * @param rotational_acceleration The new rotational acceleration value.
 */
void MovableEntity::SetRotationalAcceleration(double rotational_acceleration) {
  rotational_acceleration = rotational_acceleration;
}

/*!
 * @brief Sets the linear velocity of the entity.
 *
 * @param velocity The new linear velocity value.
 */
void MovableEntity::SetVelocity(double velocity) { velocity_ = velocity; }

/*!
 * @brief Sets the angle of the linear velocity.
 *
 * @param angle The new angle for the linear velocity, measured in radians.
 */
void MovableEntity::SetVelocityAngle(double angle) { velocity_angle_ = angle; }

/*!
 * @brief Sets the rotational velocity of the entity.
 *
 * @param rotational_velocity The new rotational velocity value.
 */
void MovableEntity::SetRotationalVelocity(double rotational_velocity) {
  rotational_velocity_ = rotational_velocity;
}

/*!
 * @brief Calculates the forward friction experienced by the entity.
 *
 * @return The calculated forward friction.
 */
double MovableEntity::GetForwardFriction() const {
  return GetVelocity() * sqrt(GetSize()) *
         settings::environment::kFrictionalCoefficient *
         (1 + strafing_difficulty * fabs(sin(GetVelocityAngle())));
}

/*!
 * @brief Calculates the effective angle of acceleration considering friction.
 *
 * @return The effective acceleration angle.
 */
double MovableEntity::GetEffectiveAccelerationAngle() const {
  // angle using the friction angle (dependent on velocity) and the acceleration
  double accel = GetAcceleration();
  double accel_angle = GetAccelerationAngle();
  double f = GetForwardFriction();
  double f_angle = GetVelocityAngle() + M_PI;
  return atan2(accel * sin(accel_angle) + f * sin(f_angle),
               accel * cos(accel_angle) + f * cos(f_angle));
}

/*!
 * @brief Calculates the effective forward acceleration considering friction.
 *
 * @return The effective forward acceleration.
 */
double MovableEntity::GetEffectiveForwardAcceleration() const {
  double acceleration = GetAcceleration();
  double friction = GetForwardFriction();
  // return the acceleration magnitude using the cosine formula
  return sqrt(pow(acceleration, 2) + pow(friction, 2) +
              2 * acceleration * friction *
                  cos(acceleration_angle - GetVelocityAngle()));
}

/*!
 * @brief Calculates the rotational friction experienced by the entity.
 *
 * @return The calculated rotational friction.
 */
double MovableEntity::GetRotationalFriction() const {
  return GetRotationalVelocity() * GetSize() *
         settings::environment::kFrictionalCoefficient;
}

/*!
 * @brief Calculates the effective rotational acceleration considering friction.
 *
 * @return The effective rotational acceleration.
 */
double MovableEntity::GetEffectiveRotationalAcceleration() const {
  return rotational_acceleration - GetRotationalFriction();
}

/*!
 * @brief Updates the velocities (linear and rotational) based on acceleration,
 * friction, and time.
 *
 * @param deltaTime The time interval over which to update the velocities.
 */
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

/*!
 * @brief Updates the position of the entity based on its velocity, orientation,
 * and time.
 *
 * @param deltaTime The time interval over which to update the position.
 * @param kMapWidth Width of the map for wrapping around.
 * @param kMapHeight Height of the map for wrapping around.
 */
void MovableEntity::Move(double deltaTime, const double kMapWidth,
                         const double kMapHeight) {
  double orientation = GetOrientation();

  double delta_x = velocity_ * cos(orientation + velocity_angle_) * deltaTime;
  double delta_y = velocity_ * sin(orientation + velocity_angle_) * deltaTime;

  // Update position
  auto [current_x, current_y] = GetCoordinates();
  double new_x = current_x + delta_x;

  double new_y = std::fmod(current_y + delta_y, kMapHeight);

  SetCoordinates(new_x, new_y, kMapWidth, kMapHeight);
}

/*!
 * @brief Updates the orientation of the entity based on its rotational velocity
 * and time.
 *
 * @param deltaTime The time interval over which to update the orientation.
 */
void MovableEntity::Rotate(double deltaTime) {
  double new_orientation =
      GetOrientation() + (GetRotationalVelocity() * deltaTime);
  SetOrientation(new_orientation);
}

/*!
 * @brief Handles the collision of this entity with another entity.
 *
 * @param other_entity The other entity involved in the collision.
 * @param kMapWidth Width of the map for collision calculations.
 * @param kMapHeight Height of the map for collision calculations.
 */
void MovableEntity::OnCollision(Entity &other_entity, double const kMapWidth,
                                double const kMapHeight) {
  Entity::OnCollision(other_entity, kMapWidth, kMapHeight);
}
