#ifndef REPRODUCTION_H
#define REPRODUCTION_H

#include <limits>
#include <optional>

#include "mutable.h"
#include "neat/neat-genome.h"

class Creature;
class Egg;

class ReproductiveSystem {
 public:
  virtual ~ReproductiveSystem() = default;

  virtual bool IsMale() const { return false; };
  virtual bool IsFemale() const { return false; };
  virtual bool IsPregnant() const { return false; }
  virtual bool ReadyToProcreate() const;
  virtual void Update(double delta_time);

  double GetMaturityAge() const { return maturity_age_; };
  double GetReproductionCooldown() const { return reproduction_cooldown_; };
  double ReadyToReproduceAt() const { return ready_to_reproduce_at_; }

 protected:
  ReproductiveSystem(){};

  double maturity_age_;
  double reproduction_cooldown_;

  double age_;
  double ready_to_reproduce_at_;

  virtual double MaturityAge(const Mutable* const mutables) const;
  virtual double ReproductionCooldown(const Mutable* const mutables) const = 0;

  virtual void ResetReproductionClock();
};

class MaleReproductiveSystem : public ReproductiveSystem {
 public:
  MaleReproductiveSystem(const Mutable* const mutables);
  MaleReproductiveSystem(Mutable mutables)
      : MaleReproductiveSystem(&mutables){};
  MaleReproductiveSystem() : MaleReproductiveSystem(Mutable()) {}

  bool IsMale() const override { return true; }
  void MateWithFemale();

 protected:
  virtual double ReproductionCooldown(
      const Mutable* const mutables) const override;
};

struct GestatingEgg {
  neat::Genome genome;
  Mutable mutables;
  int generation;
  double age;
  double incubation_time;

  GestatingEgg(neat::Genome genome, Mutable mutables, int generation);
};

class FemaleReproductiveSystem : public ReproductiveSystem {
 public:
  FemaleReproductiveSystem(const Mutable* const mutables);
  FemaleReproductiveSystem(Mutable mutables)
      : FemaleReproductiveSystem(&mutables){};
  FemaleReproductiveSystem() : FemaleReproductiveSystem(Mutable()) {}

  bool IsFemale() const override { return true; }
  bool IsPregnant() const override { return egg_.has_value(); };
  GestatingEgg GetEgg() const { return egg_.value(); }

  virtual void Update(double delta_time) override;
  void MateWithMale(const Creature* const father, const Creature* const mother);
  bool CanBirth() const;
  Egg GiveBirth(const std::pair<double, double>& coordinates);

 protected:
  virtual double ReproductionCooldown(
      const Mutable* const mutables) const override;

 protected:
  std::optional<GestatingEgg> egg_;  // gestating egg
  double gestation_ratio_to_incubation_;
};

#endif  // REPRODUCTION_H
