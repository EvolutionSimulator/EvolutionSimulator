#include "egg.h"

Egg::Egg(const GestatingEgg& gestating_egg,
         const std::pair<double, double>& coordinates)
    : Food(coordinates.first, coordinates.second, 0, 0),
      AliveEntity(gestating_egg.genome, gestating_egg.mutables),
      generation_(gestating_egg.generation),
      incubation_time_(gestating_egg.incubation_time) {
  type_ = egg;
  age_ = gestating_egg.age;
  Update(0);
}

double Egg::GetIncubationTime() const { return incubation_time_; }

void Egg::Update(double delta_time) {
  if (AliveEntity::GetState() == Dead) {
    return;
  }

  age_ += delta_time;

  Food::SetSize(age_ / incubation_time_ * AliveEntity::GetMutable().GetBabySize());
  Food::SetNutritionalValue(settings::environment::kEggNutritionalValue *
                            Food::GetSize());
}

void Egg::Break() { AliveEntity::SetState(Dead); }

Creature Egg::Hatch() {
  if (AliveEntity::GetState() == Dead) {
    throw std::runtime_error("Cannot hatch a dead egg");
  }
  if (age_ < incubation_time_) {
    throw std::runtime_error("Cannot hatch an egg that has not incubated");
  }

  Creature creature = Creature(genome_, mutable_);
  auto coordinates = Food::GetCoordinates();
  creature.SetCoordinatesNoWrap(coordinates.first, coordinates.second);
  creature.SetGeneration(generation_);
  return creature;
}