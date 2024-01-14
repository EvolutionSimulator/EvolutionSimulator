#include "grabbing_entity.h"
#include "creature.h"

#include <math.h>

GrabbingEntity::GrabbingEntity() {
  // Constructor implementation
}

/*double GrabbingEntity::GetEffectiveAccelerationAngle() const {}

double GrabbingEntity::GetEffectiveForwardAcceleration() const {
  // Implementation
}

double GrabbingEntity::GetEffectiveRotationalAcceleration() const {
  // Implementation
}*/

std::unordered_set<std::shared_ptr<MovableEntity>> GrabbingEntity::GetEntities() {
  std::unordered_set<std::shared_ptr<MovableEntity>> entities;
  entities.insert(std::make_shared<MovableEntity>(*this));
  for (auto entity : grabbing_entities_) {
    if (entity->GetID() == this->GetID()) continue;

    if (std::dynamic_pointer_cast<GrabbingEntity>(entity) != nullptr) {
      for (auto entity2 : std::dynamic_pointer_cast<GrabbingEntity>(entity)->GetEntities()) {
        entities.insert(entity2);
      }
    }
  }
  return entities;
}

std::pair<double, double> GrabbingEntity::GetCentreOfMass() {
  std::unordered_set<std::shared_ptr<MovableEntity>> entities = GetEntities();
  std::pair<double, double> centre_of_mass = {0, 0};
  for (auto& entity : entities) {
    centre_of_mass.first +=
        entity->GetCoordinates().first * pow(entity->GetSize(), 2);
    centre_of_mass.second +=
        entity->GetCoordinates().second * pow(entity->GetSize(), 2);
  }
  double total_mass = GetTotalMass();
  centre_of_mass.first /= total_mass;
  centre_of_mass.second /= total_mass;
  return centre_of_mass;
}

double GrabbingEntity::GetTotalMass() {
  std::unordered_set<std::shared_ptr<MovableEntity>> entities = GetEntities();
  double total_mass = 0;
  for (auto& entity : entities) {
    total_mass += pow(entity->GetSize(), 2);
  }
  return total_mass;
}

std::pair<double, double> GrabbingEntity::GetTotalForwardAccelComps() {
  std::unordered_set<std::shared_ptr<MovableEntity>> entities = GetEntities();
  std::pair<double, double> total_forward_accel = {0, 0};
  double total_mass = GetTotalMass();
  for (auto& entity : entities) {
    total_forward_accel.first +=
        pow(entity->GetSize(), 2) * entity->GetAcceleration() *
        cos(entity->GetAccelerationAngle() + entity->GetOrientation());
    total_forward_accel.second +=
        pow(entity->GetSize(), 2) * entity->GetAcceleration() *
        sin(entity->GetAccelerationAngle() + entity->GetOrientation());
  }
  return {total_forward_accel.first / total_mass,
          total_forward_accel.second / total_mass};
}

double GrabbingEntity::GetTotalForwardAccel() {
  std::pair<double, double> comps = GetTotalForwardAccelComps();
  return sqrt(pow(comps.first, 2) + pow(comps.second, 2));
}

double GrabbingEntity::GetTotalForwardAccelAngle() {
  std::pair<double, double> comps = GetTotalForwardAccelComps();
  return atan2(comps.second, comps.first);
}

double GrabbingEntity::GetTotalRotAccel() {
  std::unordered_set<std::shared_ptr<MovableEntity>> entities = GetEntities();
  std::pair<double, double> centre_of_mass = GetCentreOfMass();
  double total_rot_accel = 0;
  for (auto& entity : entities) {
    double cc_distance =
        sqrt(pow(entity->GetCoordinates().first - centre_of_mass.first, 2) +
             pow(entity->GetCoordinates().second - centre_of_mass.second, 2));
    double cc_angle =
        atan2(entity->GetCoordinates().second - centre_of_mass.second,
              entity->GetCoordinates().first -
                  centre_of_mass.first);  // respective to horizontal, clockwise
    double forward_componenet =
        cc_distance * entity->GetAcceleration() *
        sin(GetOrientation() + GetAccelerationAngle() - cc_angle);
    double torque1 =
        -(cc_distance - entity->GetSize()) * (rotational_acceleration_ / entity->GetSize()) / 2;
    double torque2 =
        (cc_distance + entity->GetSize()) * (rotational_acceleration_ / entity->GetSize()) / 2;
    total_rot_accel +=
        entity->GetSize() * (torque1 + torque2 + forward_componenet);
  }
  return total_rot_accel / GetTotalMass();
}

void GrabbingEntity::UpdateEntityVelocities() {
  // Implementation
}

std::shared_ptr<MovableEntity> GrabbingEntity::GetGrabbedEntity(){
    return grabbed_entity_;
}

void GrabbingEntity::SetGrabbedEntity(std::shared_ptr<MovableEntity>movable_entity){
    grabbed_entity_ = movable_entity;
}

void GrabbingEntity::AddToGrabbingEntities(std::shared_ptr<MovableEntity>movable_entity){
    grabbing_entities_.insert(movable_entity);
}
