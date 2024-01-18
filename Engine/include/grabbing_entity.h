#ifndef GRABBING_ENTITY_H
#define GRABBING_ENTITY_H
#include <unordered_set>

#include "movable_entity.h"

class GrabbingEntity : virtual public MovableEntity {
 public:
  GrabbingEntity();
  double GetEffectiveAccelerationAngle() const override;
  double GetEffectiveForwardAcceleration() const override;
  std::pair<double, double> GetEffectiveAccelerationPair() const;
  double GetEffectiveRotationalAcceleration() const override;

  double GetForwardFriction() const override;
  double GetRotationalFriction() const override;

  std::unordered_set<GrabbingEntity*> FindGrabEntities() const;
  std::unordered_set<GrabbingEntity*> GetGrabEntities() const;
  std::pair<double, double> CalcCentreOfMass() const;
  std::pair<double, double> GetCentreOfMass() const;
  double CalcTotalMass() const;
  double GetTotalMass() const;

  void UpdateEntityVelocities();
  void UpdateVelocities(double deltaTime) override;
  void Move(double deltaTime, const double kMapWidth,
            const double kMapHeight) override;
  void Rotate(double deltaTime, double kMapWidth, double kMapHeight);

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

  void AddToGrabAffected(
      GrabbingEntity*
          movable_entity_);  // Adding the entity that is affected by me to the
                             // set of my grab affected entities

  MovableEntity* GetGrabbedEntity() const;

  void SetAffectedByGrabbedEntity(bool affected);

  void SetGrabValues();

 private:
  MovableEntity* grabbed_entity_{nullptr};
  std::unordered_set<GrabbingEntity*>
      grabbed_by_;  // entities that have grabbed this one
  bool affected_by_grabbed_entity_{false};
  std::unordered_set<GrabbingEntity*>
      grab_affected_entities_;  // entities in the chain
  double total_mass_{0};        // total mass of the entity chain
  std::pair<double, double> centre_of_mass_{
      0, 0};  // centre of mass of the entity chain
};

#endif  // GRABBING_ENTITY_H
