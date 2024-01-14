#ifndef REPRODUCTION_H
#define REPRODUCTION_H

#include <limits>
#include <optional>

#include "creature.h"
#include "egg.h"
#include "mutable.h"
#include "neat/neat-genome.h"
#include "simulationdata.h"

enum GenderType {
  kMale,
  kFemale,
};

class ReproductiveSystem {
 public:
  ReproductiveSystem(double maturity_age, double reproduction_cooldown)
      : maturity_age_(maturity_age),
        age_(0),
        reproduction_cooldown_(reproduction_cooldown),
        last_reproduced_age_(std::numeric_limits<double>::min()){};

  virtual ~ReproductiveSystem() = default;

  virtual GenderType Gender() const = 0;
  virtual bool IsPregnant() const { return false; }
  virtual bool ReadyToProcreate() const = 0;

  void SetAge(double age) { age_ = age; };
  void SetReproductionCooldown(double reproduction_cooldown) {
    reproduction_cooldown_ = reproduction_cooldown;
  };
  double GetReproductionCooldown() const { return reproduction_cooldown_; };
  double GetLastReproducedAge() const { return last_reproduced_age_; };

 protected:
  ReproductiveSystem(double reproduction_cooldown)
      : reproduction_cooldown_(reproduction_cooldown),
        last_reproduced_age_(std::numeric_limits<double>::min()){};

  double maturity_age_;
  double age_;
  double reproduction_cooldown_;
  double last_reproduced_age_;
};

class MaleReproductiveSystem : public ReproductiveSystem {
 public:
  MaleReproductiveSystem(double maturity_age, double reproduction_cooldown)
      : ReproductiveSystem(maturity_age, reproduction_cooldown){};
  GenderType Gender() const override { return kMale; };
  void Mate();
  bool ReadyToProcreate() const override;
};

class FemaleReproductiveSystem : public ReproductiveSystem {
 public:
  FemaleReproductiveSystem(double maturity_age, double reproduction_cooldown)
      : ReproductiveSystem(maturity_age, reproduction_cooldown),
        last_mated_at(0),
        carrying_offspring_(false),
        offspring_genome_(0, 0){};

  GenderType Gender() const override { return kFemale; };
  double GetLastMatedAt() const { return last_mated_at; };
  bool IsCarryingOffspring() const { return carrying_offspring_; };
  bool ReadyToProcreate() const override;

  void Mate(const neat::Genome& father_genome, const Mutable& father_mutable,
            double father_energy, int father_generation,
            const neat::Genome& mother_genome, const Mutable& mother_mutable,
            double mother_energy, int mother_generation);

 protected:
  void FinishBirth();

  double last_mated_at;
  bool carrying_offspring_;

  // TODO: Remove this and fully use Egg
  neat::Genome offspring_genome_;
  Mutable offspring_mutable_;
  int offspring_generation_;

  Egg egg;
};

class FemaleCreatureBirthingSystem : public FemaleReproductiveSystem {
 public:
  FemaleCreatureBirthingSystem(double maturity_age,
                               double reproduction_cooldown,
                               double pregnancy_time)
      : FemaleReproductiveSystem(maturity_age, reproduction_cooldown),
        pregnancy_time_(pregnancy_time){};

  void SetPregnancyTime(double pregnancy_time) {
    pregnancy_time_ = pregnancy_time;
  };
  double GetPregnancyTime() const { return pregnancy_time_; };
  bool IsPregnant() const override;

  std::optional<Creature> AttemptBirthCreature(
      double const kMapWidth, double const kMapHeight);

 protected:
  double pregnancy_time_;
};

class FemaleEggLayingSystem : public FemaleReproductiveSystem {
 public:
  FemaleEggLayingSystem(double maturity_age, double reproduction_cooldown)
      : FemaleReproductiveSystem(maturity_age, reproduction_cooldown){};

  void SetIncubationTime(double incubation_time) {
    incubation_time_ = incubation_time;
  };
  double GetIncubationTime() const { return incubation_time_; };

  std::optional<Egg> AttemptLayEgg(double const kMapWidth,
                                   double const kMapHeight);

 protected:
  double incubation_time_;
};

bool CanMate(const ReproductiveSystem& first_system,
             const ReproductiveSystem& second_system);

#endif  // REPRODUCTION_H
