#ifndef REPRODUCTION_H
#define REPRODUCTION_H

#include <limits>
#include <optional>
#include <memory>

#include "entity/creature/mutable.h"
#include "neat/genome.h"
#include "entity/alive_entity.h"

class Creature;
class Egg;

class ReproductiveSystem : virtual public AliveEntity {
 public:
  ReproductiveSystem(neat::Genome genome, Mutable mutables);
  virtual ~ReproductiveSystem() = default;

  virtual bool IsMale() const { return false; };
  virtual bool IsFemale() const { return false; };
  virtual bool ReadyToProcreate() const;
  virtual void Update(double delta_time);

  double GetMaturityAge() const { return maturity_age_; };
  double GetReproductionCooldown() const { return reproduction_cooldown_; };
  double ReadyToReproduceAt() const { return ready_to_reproduce_at_; }
  bool WaitingToReproduce() const { return waiting_to_reproduce_; }

  void SetWaitingToReproduce(bool value);

  void IncreaseOffspringNumber();
  int GetOffspringNumber();
 protected:

  double maturity_age_;
  double reproduction_cooldown_;

  double ready_to_reproduce_at_;

  bool waiting_to_reproduce_;

  int offspring_number_;

  virtual double MaturityAge(const Mutable* const mutables) const;

  virtual void ResetReproductionClock();
};

class MaleReproductiveSystem : virtual public ReproductiveSystem {
 public:
  MaleReproductiveSystem(neat::Genome genome, Mutable mutables);

  bool IsMale() const override { return true; }
  void MateWithFemale();
  void MaleAfterMate();
};

class GestatingEgg {
public:
  neat::Genome genome;
  Mutable mutables;
  int generation;
  double age;
  double incubation_time;

  GestatingEgg(neat::Genome genome, Mutable mutables, int generation);
};

class FemaleReproductiveSystem : virtual public ReproductiveSystem {
 public:
  FemaleReproductiveSystem(neat::Genome genome, Mutable mutables);

  bool IsFemale() const override { return true; }
  bool IsPregnant() const  { return egg_.has_value(); };
  GestatingEgg GetEgg() const { return egg_.value(); }
  void FemaleAfterMate();

  virtual void Update(double delta_time) override;
  void MateWithMale(const std::shared_ptr<Creature> father, const std::shared_ptr<Creature> mother);
  bool CanBirth() const;
  std::shared_ptr<Egg> GiveBirth(const std::pair<double, double>& coordinates);

 protected:
  std::optional<GestatingEgg> egg_;  // gestating egg
  double gestation_ratio_to_incubation_;
  double pregnancy_hardship_;
};

#endif  // REPRODUCTION_H
