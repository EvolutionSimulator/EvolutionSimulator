#include "reproduction.h"

#include <limits>

#include "creature.h"
#include "egg.h"

bool ReproductiveSystem::ReadyToProcreate() const {
  return age_ >= ready_to_reproduce_at_;
};

void ReproductiveSystem::Update(double delta_time) { age_ += delta_time; }

void ReproductiveSystem::ResetReproductionClock() {
  ready_to_reproduce_at_ = age_ + reproduction_cooldown_;
}

double ReproductiveSystem::MaturityAge(const Mutable* const mutables) const {
  return mutables->Complexity() *
         (1 + mutables->GetMaxSize() - mutables->GetBabySize()) *
         settings::environment::kMaturityAgeMultiplier;
}

MaleReproductiveSystem::MaleReproductiveSystem(const Mutable* const mutables) {
  age_ = 0;
  maturity_age_ = MaturityAge(mutables),
  reproduction_cooldown_ = ReproductionCooldown(mutables),
  ready_to_reproduce_at_ = maturity_age_;
}

double MaleReproductiveSystem::ReproductionCooldown(
    const Mutable* const mutables) const {
  return 0;
}

void MaleReproductiveSystem::MateWithFemale() {
  if (not ReadyToProcreate())
    throw std::runtime_error("Not ready to procreate");
  ResetReproductionClock();
};

GestatingEgg::GestatingEgg(neat::Genome genome, Mutable mutables,
                           int generation)
    : genome(genome),
      mutables(mutables),
      generation(generation),
      age(0),
      incubation_time(mutables.Complexity() *
                      settings::environment::kEggIncubationTimeMultiplier) {}

FemaleReproductiveSystem::FemaleReproductiveSystem(
    const Mutable* const mutables)
    : egg_() {
  age_ = 0;
  maturity_age_ = MaturityAge(mutables),
  reproduction_cooldown_ = ReproductionCooldown(mutables),
  ready_to_reproduce_at_ = maturity_age_;
  gestation_ratio_to_incubation_ = mutables->GetGestationRatioToIncubation();
}

void FemaleReproductiveSystem::Update(double delta_time) {
  ReproductiveSystem::Update(delta_time);
  if (egg_.has_value()) {
    egg_->age += delta_time;
  }
}

void FemaleReproductiveSystem::MateWithMale(const Creature* const father,
                                            const Creature* const mother) {
  if (not ReadyToProcreate())
    throw std::runtime_error("Not ready to procreate");

  ready_to_reproduce_at_ = std::numeric_limits<double>::max();

  neat::Genome offspring_genome_(0, 0);
  Mutable offspring_mutable_;
  int offspring_generation_;

  if (father->GetEnergy() > mother->GetEnergy()) {
    offspring_genome_ =
        neat::Crossover(father->GetGenome(), mother->GetGenome());
    offspring_mutable_ =
        MutableCrossover(father->GetMutable(), mother->GetMutable());
    offspring_generation_ = father->GetGeneration() + 1;
  } else {
    offspring_genome_ =
        neat::Crossover(mother->GetGenome(), father->GetGenome());
    offspring_mutable_ =
        MutableCrossover(mother->GetMutable(), father->GetMutable());
    offspring_generation_ = mother->GetGeneration() + 1;
  }

  offspring_genome_.Mutate();
  offspring_genome_.Mutate();
  offspring_mutable_.Mutate();
  offspring_mutable_.Mutate();

  egg_.emplace(offspring_genome_, offspring_mutable_, offspring_generation_);
};

bool FemaleReproductiveSystem::CanBirth() const {
  return egg_.has_value() and
         egg_->age / egg_->incubation_time >= gestation_ratio_to_incubation_;
}

Egg FemaleReproductiveSystem::GiveBirth(
    const std::pair<double, double>& coordinates) {
  if (not CanBirth()) {
    throw std::runtime_error("Egg not ready to hatch");
  }

  GestatingEgg egg = egg_.value();
  egg_.reset();

  ResetReproductionClock();
  return Egg(egg, coordinates);
}

double FemaleReproductiveSystem::ReproductionCooldown(
    const Mutable* const mutables) const {
  return mutables->Complexity() * 0.5;
}