#ifndef MOVABLE_ENTITY_H
#define MOVABLE_ENTITY_H

#include "entity.h"

class MovableEntity : virtual public Entity {
 public:
  MovableEntity();
  virtual ~MovableEntity() override {}

  double GetAcceleration() const;
  double GetAccelerationAngle()
      const;  // Angle relative to orientation of entity
  double GetRotationalAcceleration() const;
  double GetVelocity() const;
  double GetVelocityAngle() const;  // Angle relative to orientation of entity
  double GetRotationalVelocity() const;

  void SetAcceleration(double acceleration);
  void SetAccelerationAngle(
      double angle);  // Angle relative to orientation of entity
  void SetRotationalAcceleration(double rotational_acceleration);
  void SetVelocity(double velocity);
  void SetVelocityAngle(
      double angle);  // Angle relative to orientation of entity
  void SetRotationalVelocity(double rotational_velocity);

  double GetForwardFriction() const;
  double GetEffectiveAccelerationAngle() const;
  double GetEffectiveForwardAcceleration() const;
  double GetRotationalFriction() const;
  double GetEffectiveRotationalAcceleration() const;

  void UpdateVelocities(double deltaTime);
  void Move(double deltaTime, const double kMapWidth, const double kMapHeight);
  void Rotate(double deltaTime);

  virtual void OnCollision(Entity &other_entity, double const kMapWidth,
                           double const kMapHeight);

 protected:
  double acceleration_, acceleration_angle_, rotational_acceleration_;
  double velocity_, velocity_angle_, rotational_velocity_;
  double strafing_difficulty_;
  double frictional_coefficient_;
};

#endif  // CREATURE_HPP
