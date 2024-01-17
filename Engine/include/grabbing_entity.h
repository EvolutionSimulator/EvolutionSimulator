#ifndef GRABBING_ENTITY_H
#define GRABBING_ENTITY_H
#include <unordered_set>

#include "movable_entity.h"

class GrabbingEntity : virtual public MovableEntity {
 public:
  GrabbingEntity();
  /*double GetEffectiveAccelerationAngle() const override;
  double GetEffectiveForwardAcceleration() const override;
  double GetEffectiveRotationalAcceleration() const override;*/

  double GetForwardFriction() const override;
  double GetRotationalFriction() const override;

  std::unordered_set<GrabbingEntity*> GetGrabbedBy() const;
  std::pair<double, double> GetCentreOfMass() const;
  double GetTotalMass() const;
  void UpdateEntityVelocities() const;

  std::pair<double, double> GetTotalForwardAccelComps() const;
  double GetTotalForwardAccel() const;
  double GetTotalForwardAccelAngle() const;
  double GetTotalRotAccel() const;

  void SetGrabbedEntity(
      MovableEntity* movable_entity);  // Setting the entity i am grabbing as my
                                       // grabbed entity
  void AddToGrabbedBy(
      GrabbingEntity*
          movable_entity_);  // Adding the entity that is grabbing me to the set
                             // of my grabbing entities

  MovableEntity* GetGrabbedEntity() const;
  bool grabbing_;  /*! Indicates whether creature is grabbing or not*/

 private:
  MovableEntity* grabbed_entity_{nullptr};
  std::unordered_set<GrabbingEntity*>
      grabbed_by_;  // entities that have grabbed this one
  bool affected_by_grabbed_entity_{false};

};

#endif  // GRABBING_ENTITY_H
