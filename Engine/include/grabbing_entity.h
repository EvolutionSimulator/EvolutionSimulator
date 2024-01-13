#ifndef GRABBING_ENTITY_H
#define GRABBING_ENTITY_H
#include <unordered_set>

#include "movable_entity.h"

class GrabbingEntity : virtual public MovableEntity {
 public:
  GrabbingEntity();
  double GetEffectiveAccelerationAngle() const override;
  double GetEffectiveForwardAcceleration() const override;
  double GetEffectiveRotationalAcceleration() const override;

  std::unordered_set<MovableEntity*> GetEntities();
  std::pair<double, double> GetCentreOfMass();
  double GetTotalMass();
  void UpdateEntityVelocities();

  std::pair<double, double> GetTotalForwardAccelComps();
  double GetTotalForwardAccel();
  double GetTotalForwardAccelAngle();
  double GetTotalRotAccel();

 private:
  MovableEntity* grabbed_entity_{nullptr};
  std::unordered_set<MovableEntity*>
      grabbing_entities_;  // entities that have grabbed this one
  bool affected_by_grabbed_entity_{false};
};

#endif  // GRABBING_ENTITY_H
