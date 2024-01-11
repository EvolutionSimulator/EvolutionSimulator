#ifndef GRABBING_ENTITY_H
#define GRABBING_ENTITY_H
#include "movable_entity.h"

class grabbing_entity : virtual public MovableEntity {
 public:
  grabbing_entity();
  double GetEffectiveAccelerationAngle() const override;
  double GetEffectiveForwardAcceleration() const override;
  double GetEffectiveRotationalAcceleration() const override;

  double CalcGrabForce() const;
  double CalcGrabForceAngle() const;
  double CalcGrabRotForce() const;

  void SetGrabForce(double force);
  void SetGrabForceAngle(double force_angle);
  void SetGrabRotForce(double force);
  double GetGrabForce() const;
  double GetGrabForceAngle() const;
  double GetGrabRotForce() const;

  void SetGrabbedForce(double force);
  void SetGrabbedForceAngle(double force_angle);
  void SetGrabbedRotForce(double force);
  double GetGrabbedForce() const;
  double GetGrabbedForceAngle() const;
  double GetGrabbedRotForce() const;

 private:
  MovableEntity* grabbed_entity = nullptr;
  // extra forces due to grabbed entity
  double grab_force_ = 0, grab_force_angle_ = 0, grab_rot_force = 0;
  // extra forces due to being grabbed by other entities
  double grabbed_force_ = 0, grabbed_force_angle_ = 0, grabbed_rot_force = 0;
  // status to check whether the entity has been affected by the grabbed_entity
  // for the case in which 2 entities grabbed each other
  bool affected_by_grabbed_entity = false;
};

#endif  // GRABBING_ENTITY_H
