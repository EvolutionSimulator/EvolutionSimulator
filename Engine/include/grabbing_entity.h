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

  std::unordered_set<std::shared_ptr<MovableEntity>> GetEntities();
  std::pair<double, double> GetCentreOfMass();
  double GetTotalMass();
  void UpdateEntityVelocities();

  std::pair<double, double> GetTotalForwardAccelComps();
  double GetTotalForwardAccel();
  double GetTotalForwardAccelAngle();
  double GetTotalRotAccel();

  void SetGrabbedEntity(std::shared_ptr<MovableEntity> movable_entity); //Setting the entity i am grabbing as my grabbed entity
  void AddToGrabbingEntities(std::shared_ptr<MovableEntity> movable_entity_); //Adding the entity that is grabbing me to the set of my grabbing entities

  std::shared_ptr<MovableEntity> GetGrabbedEntity();
  bool grabbing_;  /*! Indicates whether creature is grabbing or not*/

 private:
  std::shared_ptr<MovableEntity> grabbed_entity_{nullptr};
  std::unordered_set<std::shared_ptr<MovableEntity>>
      grabbing_entities_;  // entities that have grabbed this one
  bool affected_by_grabbed_entity_{false};

};

#endif  // GRABBING_ENTITY_H
