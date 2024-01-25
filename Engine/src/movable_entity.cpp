#include "movable_entity.h"

#include "settings.h"
#include "math.h"
#include "geometry_primitives.h"
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
      acceleration_angle_(0),
      rotational_acceleration_(0),
      velocity_(0),
      velocity_angle_(0),
      rotational_velocity_(0),
      strafing_difficulty_(0.5),
      frictional_coefficient_(SETTINGS.environment.frictional_coefficient){}

/*!
 * @brief Returns the linear acceleration.
 */
double MovableEntity::GetAcceleration() const { return acceleration_; }

/*!
 * @brief Returns the angle of acceleration.
 */
double MovableEntity::GetAccelerationAngle() const {
  return acceleration_angle_;
}

/*!
 * @brief Returns the rotational acceleration.
 */
double MovableEntity::GetRotationalAcceleration() const {
  return rotational_acceleration_;
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
  acceleration_angle_ = angle;
}

/*!
 * @brief Sets the rotational acceleration of the entity.
 *
 * @param rotational_acceleration The new rotational acceleration value.
 */
void MovableEntity::SetRotationalAcceleration(double rotational_acceleration) {
  rotational_acceleration_ = rotational_acceleration;
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
  return GetVelocity() * sqrt(std::abs(GetSize())) *
         frictional_coefficient_ *
         (1 + strafing_difficulty_ * fabs(sin(GetVelocityAngle())));
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
  double effective_angle = atan2(accel * sin(accel_angle) + f * sin(f_angle),
                                 accel * cos(accel_angle) + f * cos(f_angle));
  return effective_angle;
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

  double acceleration_squared =
      pow(acceleration, 2) + pow(friction, 2) -
      2 * acceleration * friction *
          cos(acceleration_angle_ - GetVelocityAngle());
  if (acceleration_squared < 0) {
    return 0;
  }
  return sqrt(acceleration_squared);
}

/*!
 * @brief Calculates the rotational friction experienced by the entity.
 *
 * @return The calculated rotational friction.
 */
double MovableEntity::GetRotationalFriction() const {
  return GetRotationalVelocity() * GetSize() * frictional_coefficient_;
}

/*!
 * @brief Calculates the effective rotational acceleration considering friction.
 *
 * @return The effective rotational acceleration.
 */
double MovableEntity::GetEffectiveRotationalAcceleration() const {
  return rotational_acceleration_ - GetRotationalFriction();
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
                cos(GetOrientation() + GetEffectiveAccelerationAngle()) *
                deltaTime;
  velocity_y += GetEffectiveForwardAcceleration() *
                sin(GetOrientation() + GetEffectiveAccelerationAngle()) *
                deltaTime;
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

  double delta_x =
      GetVelocity() * cos(orientation + velocity_angle_) * deltaTime;
  double delta_y =
      GetVelocity() * sin(orientation + velocity_angle_) * deltaTime;

  // Update position
  auto [current_x, current_y] = GetCoordinates();
  double new_x = current_x + delta_x;

  double new_y = fmod(current_y + delta_y, kMapHeight);

  SetCoordinates(new_x, new_y, kMapWidth, kMapHeight);
}

/*!
 * @brief Updates the orientation of the entity based on its rotational velocity
 * and time.
 *
 * @param deltaTime The time interval over which to update the orientation.
 */
void MovableEntity::Rotate(double deltaTime) {
  OrientedAngle new_orientation =
      OrientedAngle(GetOrientation() + (GetRotationalVelocity() * deltaTime));
  SetOrientation(new_orientation.GetAngle());
}

/*!
 * @brief Handles the collision of this entity with another entity.
 *
 * @param other_entity The other entity involved in the collision.
 * @param kMapWidth Width of the map for collision calculations.
 * @param kMapHeight Height of the map for collision calculations.
 */
void MovableEntity::OnCollision(std::shared_ptr<Entity> other_entity, double const kMapWidth,
                                double const kMapHeight) {
    std::shared_ptr<MovableEntity> other_movable = std::dynamic_pointer_cast<MovableEntity>(other_entity);

    if (!other_movable) return;
    //Check if the entity is colliding with itself
    if (this->GetID() == other_movable->GetID()) return;

    // Get the coordinates and size of the other entity
    std::pair<double, double> other_coordinates = other_movable->GetCoordinates();
    double other_size = other_movable->GetSize();

    // Calculate the distance between the two entities
    double distance = GetDistance(other_movable, kMapWidth, kMapHeight);

    // If the distance is zero, return
    if (distance == 0.0) return;

    // Calculate the overlap between the two entities
    double overlap = size_ + other_size - distance;

    // Calculate the overlap in the x and y directions
    double x_overlap = overlap * (x_coord_ - other_coordinates.first) / distance;
    double y_overlap = overlap * (y_coord_ - other_coordinates.second) / distance;

    // Calculate how much should each entity move
    double total_size = std::pow(size_, 2) + std::pow(other_size, 2);
    this->SetCoordinates(x_coord_ + x_overlap*std::pow(size_, 2)/total_size,
                         y_coord_ + y_overlap*std::pow(size_, 2)/total_size,
                         kMapWidth, kMapHeight);
    other_movable->SetCoordinates(other_coordinates.first - x_overlap*std::pow(other_size, 2)/total_size,
                                 other_coordinates.second - y_overlap*std::pow(other_size, 2)/total_size,
                                 kMapWidth, kMapHeight);

    // Assert that the entities are no longer overlapping
    // assert(size_ + other_size - GetDistance(other_movable, kMapWidth,
    // kMapHeight) < 0.0001);
}

