#include "egg.h"
#include "settings.h"

Egg::Egg(const GestatingEgg& gestating_egg,
         const std::pair<double, double>& coordinates)
    : Entity(coordinates.first, coordinates.second, 0),
      AliveEntity(gestating_egg.genome, gestating_egg.mutables),
      generation_(gestating_egg.generation),
      incubation_time_(gestating_egg.incubation_time),
      nutritional_value_(SETTINGS.environment.egg_nutritional_value){
  age_ = gestating_egg.age;
  Update(0);
}

double Egg::GetIncubationTime() const { return incubation_time_; }

void Egg::Update(double delta_time) {
  if (AliveEntity::GetState() == Dead) {
    return;
  }

  age_ += delta_time;

  SetSize(age_ / incubation_time_ * AliveEntity::GetMutable().GetBabySize());
}

void Egg::Break() { AliveEntity::SetState(Dead); }

std::shared_ptr<Creature> Egg::Hatch() {
  if (AliveEntity::GetState() == Dead) {
    throw std::runtime_error("Cannot hatch a dead egg");
  }
  if (age_ < incubation_time_) {
    throw std::runtime_error("Cannot hatch an egg that has not incubated");
  }

  std::shared_ptr<Creature> creature = std::make_shared<Creature>(genome_, mutable_);
  auto coordinates = GetCoordinates();
  creature->SetCoordinates(coordinates.first, coordinates.second);
  creature->SetGeneration(generation_);
  return creature;
}
