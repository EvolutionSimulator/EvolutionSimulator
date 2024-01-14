#include "reproduction.h"

#include <limits>

#include "random.h"

void MaleReproductiveSystem::Mate() { last_reproduced_age_ = age_; };

bool MaleReproductiveSystem::ReadyToProcreate() const {
  return age_ >= maturity_age_ and
         age_ >= last_reproduced_age_ + reproduction_cooldown_;
};

bool FemaleReproductiveSystem::ReadyToProcreate() const {
  return age_ >= maturity_age_ and not carrying_offspring_ and
         age_ >= last_reproduced_age_ + reproduction_cooldown_;
};

void FemaleReproductiveSystem::Mate(const neat::Genome& father_genome,
                                    const Mutable& father_mutable,
                                    double father_energy, int father_generation,
                                    const neat::Genome& mother_genome,
                                    const Mutable& mother_mutable,
                                    double mother_energy,
                                    int mother_generation) {
  last_mated_at = age_;
  carrying_offspring_ = true;

  if (father_energy > mother_energy) {
    offspring_genome_ = neat::Crossover(father_genome, mother_genome);
    offspring_mutable_ = MutableCrossover(father_mutable, mother_mutable);
    offspring_generation_ = father_generation + 1;
  } else {
    offspring_genome_ = neat::Crossover(mother_genome, father_genome);
    offspring_mutable_ = MutableCrossover(mother_mutable, father_mutable);
    offspring_generation_ = mother_generation + 1;
  }

  offspring_genome_.Mutate();
  offspring_genome_.Mutate();
  offspring_mutable_.Mutate();
  offspring_mutable_.Mutate();
};

void FemaleReproductiveSystem::FinishBirth() {
  last_reproduced_age_ = age_;
  carrying_offspring_ = false;
};

bool FemaleCreatureBirthingSystem::IsPregnant() const {
  return IsCarryingOffspring();
};

std::optional<Creature> FemaleCreatureBirthingSystem::AttemptBirthCreature(
    double const kMapWidth, double const kMapHeight) {
  if (not IsCarryingOffspring() or
      GetLastMatedAt() + GetPregnancyTime() > age_) {
    return std::nullopt;
  }
  Creature offspring(offspring_genome_, offspring_mutable_);
  offspring.RandomInitialization(kMapWidth, kMapHeight);
  offspring.SetGeneration(offspring_generation_);

  FinishBirth();
  return offspring;
}

std::optional<Egg> FemaleEggLayingSystem::AttemptLayEgg(
    double const kMapWidth, double const kMapHeight) {
  if (not IsCarryingOffspring()) {
    return std::nullopt;
  }
  Egg egg(offspring_genome_, offspring_mutable_, offspring_generation_,
          GenerateRandomDouble(0, kMapWidth),
          GenerateRandomDouble(0, kMapWidth));

  FinishBirth();
  return egg;
}

bool CanMate(const ReproductiveSystem& first_system,
             const ReproductiveSystem& second_system) {
  return first_system.ReadyToProcreate() and
         second_system.ReadyToProcreate() and
         first_system.Gender() != second_system.Gender();
}