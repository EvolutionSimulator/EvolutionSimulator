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
  color_hue_ = mutable_.GetColor();
  Update(0);
}

double Egg::GetIncubationTime() const { return incubation_time_; }

void Egg::Update(double delta_time) {
  if (AliveEntity::GetState() == Dead) {
    return;
  }

  age_ += delta_time;

  SetSize((0.5 + age_ / incubation_time_) * AliveEntity::GetMutable().GetBabySize());
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

/*!
 * @brief Determines if the egg is compatible with a
 * creature.
 *
 * @details This function calculates the compatibility between the current
 * creature and another creature based on their genomes and mutable
 * characteristics. It sums the brain distance, derived from genome
 * compatibility, with the mutable distance, derived from mutable
 * characteristics compatibility. The creatures are considered compatible if the
 * sum is less than a predefined compatibility threshold.
 *
 * @param other_creature A reference to another `Creature` object for
 * compatibility comparison.
 * @return bool Returns `true` if the sum of brain and mutable distances is less
 * than the compatibility threshold, indicating compatibility; otherwise returns
 * `false`.
 */
bool Egg::CompatibleWithCreature(neat::Genome genome, Mutable mutables) {
  double brain_distance = this->GetGenome().CompatibilityBetweenGenomes(genome);
  double mutable_distance = this->GetMutable().CompatibilityBetweenMutables(mutables);
  return brain_distance + mutable_distance < SETTINGS.compatibility.compatibility_threshold;
}
