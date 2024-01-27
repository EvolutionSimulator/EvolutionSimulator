#include "reproduction.h"

#include <limits>

#include "creature.h"
#include "egg.h"
#include "settings.h"

ReproductiveSystem::ReproductiveSystem(neat::Genome genome, Mutable mutables)
    : AliveEntity(genome, mutables),
      reproduction_cooldown_(0.0),
      waiting_to_reproduce_(false){
    maturity_age_ = MaturityAge(&mutables),
    ready_to_reproduce_at_ = maturity_age_;
}


bool ReproductiveSystem::ReadyToProcreate() const {
  return age_ >= ready_to_reproduce_at_;
};

void ReproductiveSystem::Update(double delta_time) { }

void ReproductiveSystem::ResetReproductionClock() {
  ready_to_reproduce_at_ = age_ + reproduction_cooldown_;
}

double ReproductiveSystem::MaturityAge(const Mutable* const mutables) const {
  return mutables->Complexity() *
         (1 + mutables->GetMaxSize() - mutables->GetBabySize()) *
         SETTINGS.environment.maturity_age_multiplier;
}

void ReproductiveSystem::SetWaitingToReproduce(bool value){
    waiting_to_reproduce_ = value;
}

MaleReproductiveSystem::MaleReproductiveSystem(neat::Genome genome, Mutable mutables)
    : ReproductiveSystem(genome, mutables),
      AliveEntity(genome, mutables){ //Not really sure why but this is required
  reproduction_cooldown_ = 0;
}

void MaleReproductiveSystem::MateWithFemale() {
  if (not ReadyToProcreate())
    throw std::runtime_error("Not ready to procreate");
  ResetReproductionClock();
};

void MaleReproductiveSystem::MaleAfterMate() {
    SetEnergy(energy_ - max_energy_ * SETTINGS.environment.male_reproduction_cost);
}

GestatingEgg::GestatingEgg(neat::Genome genome, Mutable mutables,
                           int generation)
    : genome(genome),
      mutables(mutables),
      generation(generation),
      age(0),
      incubation_time(mutables.Complexity() *
                      SETTINGS.environment.egg_incubation_time_multiplier) {}

FemaleReproductiveSystem::FemaleReproductiveSystem(neat::Genome genome, Mutable mutables)
    : ReproductiveSystem(genome, mutables),
      AliveEntity(genome, mutables), //Not really sure why but this is required
      egg_(),
      gestation_ratio_to_incubation_(mutables.GetGestationRatioToIncubation()){
    reproduction_cooldown_ = 0.5 * mutables.Complexity();
}

void FemaleReproductiveSystem::Update(double delta_time) {
  ReproductiveSystem::Update(delta_time);
  if (egg_.has_value()) {
    egg_->age += delta_time;
  }
}

void FemaleReproductiveSystem::MateWithMale(const std::shared_ptr<Creature> father,
                                            const std::shared_ptr<Creature> mother) {
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

std::shared_ptr<Egg> FemaleReproductiveSystem::GiveBirth(
    const std::pair<double, double>& coordinates) {
  if (not CanBirth()) {
    throw std::runtime_error("Egg not ready to hatch");
  }

  GestatingEgg egg = egg_.value();
  egg_.reset();

  ResetReproductionClock();
  pregnancy_hardship_ = 1;
  return std::make_shared<Egg>(egg, coordinates);
}

void FemaleReproductiveSystem::FemaleAfterMate() {
    SetEnergy(GetEnergy() - SETTINGS.physical_constraints.pregnancy_energy_factor * max_energy_);
    pregnancy_hardship_ = SETTINGS.environment.pregnancy_hardship_modifier;
}
