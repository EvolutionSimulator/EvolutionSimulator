#include "grabbing_entity.h"

#include <math.h>

#include <iostream>

#include "config.h"
#include "movable_entity.h"

GrabbingEntity::GrabbingEntity() : MovableEntity() {}

double GrabbingEntity::GetEffectiveAccelerationAngle() const {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    return MovableEntity::GetEffectiveAccelerationAngle();
  }
  double accel = GetTotalForwardAccel();
  double accel_angle = GetTotalForwardAccelAngle();
  double f = GetForwardFriction();
  double f_angle = GetVelocityAngle() + M_PI;
  double effective_angle = atan2(accel * sin(accel_angle) + f * sin(f_angle),
                                 accel * cos(accel_angle) + f * cos(f_angle));
  return effective_angle;
}

double GrabbingEntity::GetEffectiveForwardAcceleration() const {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    return MovableEntity::GetEffectiveForwardAcceleration();
  }
  double accel = GetTotalForwardAccel();
  double accel_angle = GetTotalForwardAccelAngle();
  double f = GetForwardFriction();
  double f_angle = GetVelocityAngle() + M_PI;
  double effective_accel =
      sqrt(pow(accel * sin(accel_angle) + f * sin(f_angle), 2) +
           pow(accel * cos(accel_angle) + f * cos(f_angle), 2));
  return effective_accel;
}

double GrabbingEntity::GetEffectiveRotationalAcceleration() const {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    return MovableEntity::GetEffectiveRotationalAcceleration();
  }
  double rotational_accel = GetTotalRotAccel();
  double rotational_friction = GetRotationalFriction();
  return rotational_accel - rotational_friction;
}

double GrabbingEntity::GetForwardFriction() const {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    return MovableEntity::GetForwardFriction();
  }
  double frictional_coefficient = settings::environment::kFrictionalCoefficient;
  return GetVelocity() * sqrt(GetTotalMass()) * frictional_coefficient *
         (1 + strafing_difficulty_ * fabs(sin(GetVelocityAngle())));
}

double GrabbingEntity::GetRotationalFriction() const {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    return MovableEntity::GetRotationalFriction();
  }
  double frictional_coefficient = settings::environment::kFrictionalCoefficient;
  return GetRotationalVelocity() * GetTotalMass() * frictional_coefficient;
}

std::unordered_set<GrabbingEntity*> GrabbingEntity::FindGrabEntities() const {
  std::unordered_set<GrabbingEntity*> entities;
  entities.insert(const_cast<GrabbingEntity*>(this));
  for (GrabbingEntity* entity : grabbed_by_) {
    if (entity == this) continue;

    if (dynamic_cast<const GrabbingEntity*>(entity) != nullptr) {
      for (auto& entity2 :
           dynamic_cast<GrabbingEntity*>(entity)->GetGrabEntities()) {
        entities.insert(entity2);
      }
    }
  }
  return entities;
}

std::unordered_set<GrabbingEntity*> GrabbingEntity::GetGrabEntities() const {
  return grab_affected_entities_;
}

std::pair<double, double> GrabbingEntity::CalcCentreOfMass() const {
  std::unordered_set<GrabbingEntity*> entities = GetGrabEntities();
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

std::pair<double, double> GrabbingEntity::GetCentreOfMass() const {
  return centre_of_mass_;
}

double GrabbingEntity::CalcTotalMass() const {
  std::unordered_set<GrabbingEntity*> entities = GetGrabEntities();
  double total_mass = 0;
  for (auto& entity : entities) {
    total_mass += pow(entity->GetSize(), 2);
  }
  return total_mass;
}

double GrabbingEntity::GetTotalMass() const { return total_mass_; }

std::pair<double, double> GrabbingEntity::GetTotalForwardAccelComps() const {
  std::unordered_set<GrabbingEntity*> entities = GetGrabEntities();
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

double GrabbingEntity::GetTotalRotAccel() const {
  std::unordered_set<GrabbingEntity*> entities = GetGrabEntities();
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

MovableEntity* GrabbingEntity::GetGrabbedEntity() const {
  return grabbed_entity_;
}

void GrabbingEntity::SetGrabbedEntity(MovableEntity* movable_entity) {
  grabbed_entity_ = movable_entity;
}

void GrabbingEntity::AddToGrabbedBy(GrabbingEntity* movable_entity) {
  grabbed_by_.insert(movable_entity);
}

void GrabbingEntity::AddToGrabAffected(GrabbingEntity* movable_entity) {
  grab_affected_entities_.insert(movable_entity);
}

void GrabbingEntity::SetAffectedByGrabbedEntity(bool affected) {
  affected_by_grabbed_entity_ = affected;
}

void GrabbingEntity::SetAffectedByGrabbedEnttityAll(bool affected) {
  for (GrabbingEntity* entity : grab_affected_entities_) {
    entity->SetAffectedByGrabbedEntity(affected);
  }
}

void GrabbingEntity::SetGrabValues() {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    return;
  }
  grab_affected_entities_ = FindGrabEntities();
  total_mass_ = CalcTotalMass();
  centre_of_mass_ = GetCentreOfMass();
}

void GrabbingEntity::UpdateEntityVelocities() {
  double total_v_x = 0;
  double total_v_y = 0;

  for (GrabbingEntity* entity : grab_affected_entities_) {
    total_v_x += entity->GetVelocity() * cos(entity->GetVelocityAngle());
    total_v_y += entity->GetVelocity() * sin(entity->GetVelocityAngle());
  }
  for (GrabbingEntity* entity : grab_affected_entities_) {
    entity->SetVelocity(sqrt(pow(total_v_x, 2) + pow(total_v_y, 2)));
    entity->SetVelocityAngle(atan2(total_v_y, total_v_x));
    entity->SetRotationalAcceleration(0);
  }
}

void GrabbingEntity::UpdateVelocities(double deltaTime) {
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    MovableEntity::UpdateVelocities(deltaTime);
    return;
  }
  for (GrabbingEntity* entity : grab_affected_entities_) {
    entity->SetAffectedByGrabbedEntity(true);
    double velocity_x =
        GetVelocity() * cos(GetOrientation() + GetVelocityAngle());
    double velocity_y =
        GetVelocity() * sin(GetOrientation() + GetVelocityAngle());
    velocity_x += GetEffectiveForwardAcceleration() * deltaTime *
                  cos(GetOrientation() + GetEffectiveAccelerationAngle());
    velocity_y += GetEffectiveForwardAcceleration() * deltaTime *
                  sin(GetOrientation() + GetEffectiveAccelerationAngle());
    entity->SetVelocity(sqrt(pow(velocity_x, 2) + pow(velocity_y, 2)));

    entity->SetVelocityAngle(atan2(velocity_y, velocity_x) - GetOrientation());
    entity->SetRotationalVelocity(GetRotationalVelocity() +
                                  GetEffectiveRotationalAcceleration() *
                                      deltaTime);
  }
}

void GrabbingEntity::Move(double deltaTime, const double kMapWidth,
                          const double kMapHeight) {
  std::cout << GetID() << std::endl;
  int id = GetID();
  if (affected_by_grabbed_entity_) return;
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    MovableEntity::Move(deltaTime, kMapWidth, kMapHeight);
    return;
  }
  for (GrabbingEntity* entity : grab_affected_entities_) {
    double x = entity->GetCoordinates().first;
    double y = entity->GetCoordinates().second;
    double velocity = entity->GetVelocity();
    double velocity_angle = entity->GetVelocityAngle();
    double new_x = x + velocity * deltaTime * cos(velocity_angle);
    double new_y = y + velocity * deltaTime * sin(velocity_angle);
    if (new_x < 0) {
      new_x += kMapWidth;
    } else if (new_x > kMapWidth) {
      new_x -= kMapWidth;
    }
    if (new_y < 0) {
      new_y += kMapHeight;
    } else if (new_y > kMapHeight) {
      new_y -= kMapHeight;
    }
    entity->SetCoordinates(new_x, new_y, kMapWidth, kMapHeight);
  }
}

void GrabbingEntity::Rotate(double deltaTime, double kMapWidth,
                            double kMapHeight) {
  if (affected_by_grabbed_entity_) return;
  if (!grabbed_entity_ && !grabbed_by_.size()) {
    MovableEntity::Rotate(deltaTime);
    return;
  }
  for (GrabbingEntity* entity : grab_affected_entities_) {
    double cc_distance =
        sqrt(pow(entity->GetCoordinates().first - centre_of_mass_.first, 2) +
             pow(entity->GetCoordinates().second - centre_of_mass_.second, 2));
    double cc_angle = atan2(
        entity->GetCoordinates().second - centre_of_mass_.second,
        entity->GetCoordinates().first -
            centre_of_mass_.first);  // respective to horizontal, clockwise
    double new_cc_angle = cc_angle + GetRotationalVelocity() * deltaTime;
    double x = centre_of_mass_.first + cc_distance * cos(new_cc_angle);
    double y = centre_of_mass_.second + cc_distance * sin(new_cc_angle);
    entity->SetCoordinates(x, y, kMapWidth, kMapHeight);
  }
}