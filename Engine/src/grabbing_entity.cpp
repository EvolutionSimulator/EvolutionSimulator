#include "grabbing_entity.h"

#include <math.h>

GrabbingEntity::GrabbingEntity() : MovableEntity() {}

/*
double GrabbingEntity::GetEffectiveAccelerationAngle() const {
  double accel = GetTotalForwardAccel();
  double accel_angle = GetTotalForwardAccelAngle();
  double f = GetForwardFriction();
  double f_angle = GetVelocityAngle() + M_PI;
  double effective_angle = atan2(accel * sin(accel_angle) + f * sin(f_angle),
                                 accel * cos(accel_angle) + f * cos(f_angle));
  return effective_angle;
}

double GrabbingEntity::GetEffectiveForwardAcceleration() const {
  // Implementation
}

double GrabbingEntity::GetEffectiveRotationalAcceleration() const {
  // Implementation
}*/

std::unordered_set<std::shared_ptr<GrabbingEntity>> GrabbingEntity::GetGrabbedBy() const {
  std::unordered_set<std::shared_ptr<GrabbingEntity>> entities;
  entities.insert(std::make_shared<GrabbingEntity>(*this));
  for (auto entity : grabbed_by_) {
    if (entity->GetID() == this->GetID()) continue;

    if (std::dynamic_pointer_cast<GrabbingEntity>(entity) != nullptr) {
      for (auto entity2 : std::dynamic_pointer_cast<GrabbingEntity>(entity)->GetGrabbedBy()) {
        entities.insert(entity2);
      }
    }
  }
  return entities;
}

double GrabbingEntity::GetForwardFriction() const {}

double GrabbingEntity::GetRotationalFriction() const {}

std::pair<double, double> GrabbingEntity::GetCentreOfMass() const {
  std::unordered_set<std::shared_ptr<GrabbingEntity>> entities = GetGrabbedBy();
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

double GrabbingEntity::GetTotalMass() const {
  std::unordered_set<std::shared_ptr<GrabbingEntity>> entities = GetGrabbedBy();
  double total_mass = 0;
  for (auto& entity : entities) {
    total_mass += pow(entity->GetSize(), 2);
  }
  return total_mass;
}

std::pair<double, double> GrabbingEntity::GetTotalForwardAccelComps() const {
  std::unordered_set<std::shared_ptr<GrabbingEntity>> entities = GetGrabbedBy();
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

double GrabbingEntity::GetTotalForwardAccel() const {
  std::pair<double, double> comps = GetTotalForwardAccelComps();
  return sqrt(pow(comps.first, 2) + pow(comps.second, 2));
}

double GrabbingEntity::GetTotalForwardAccelAngle() const {
  std::pair<double, double> comps = GetTotalForwardAccelComps();
  return atan2(comps.second, comps.first);
}

double GrabbingEntity::GetTotalRotAccel() const{
  std::unordered_set<std::shared_ptr<GrabbingEntity>> entities = GetGrabbedBy();
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
    double torque1 = -(cc_distance - entity->GetSize()) *
                     (rotational_acceleration_ / entity->GetSize()) / 2;
    double torque2 = (cc_distance + entity->GetSize()) *
                     (rotational_acceleration_ / entity->GetSize()) / 2;
    total_rot_accel +=
        entity->GetSize() * (torque1 + torque2 + forward_componenet);
  }
  return total_rot_accel / GetTotalMass();
}

void GrabbingEntity::UpdateEntityVelocities() const {
  // Implementation
}

std::shared_ptr<MovableEntity> GrabbingEntity::GetGrabbedEntity() const {
    return grabbed_entity_;
}

void GrabbingEntity::SetGrabbedEntity(std::shared_ptr<MovableEntity>movable_entity){
    grabbed_entity_ = movable_entity;
}

void GrabbingEntity::AddToGrabbedBy(std::shared_ptr<GrabbingEntity>movable_entity){
    grabbed_by_.insert(movable_entity);
}
