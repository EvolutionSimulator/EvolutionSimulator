#ifndef REPRODUCTION_H
#define REPRODUCTION_H

#include <limits>
#include <optional>
#include <memory>

#include "mutable.h"
#include "neat/neat-genome.h"
#include "alive_entity.h"

class Creature;
class Egg;

class ReproductiveSystem : virtual public AliveEntity {
 public:
  ReproductiveSystem(neat::Genome genome, Mutable mutables);
  virtual ~ReproductiveSystem() = default;

  virtual bool IsMale() const { return false; };
  virtual bool IsFemale() const { return false; };
  virtual bool IsPregnant() const { return false; }
  virtual bool ReadyToProcreate() const;
  virtual void Update(double delta_time);

  double GetMaturityAge() const { return maturity_age_; };
  double GetReproductionCooldown() const { return reproduction_cooldown_; };
  double ReadyToReproduceAt() const { return ready_to_reproduce_at_; }
  bool WaitingToReproduce() const { return waiting_to_reproduce_; }

  void SetWaitingToReproduce(bool value);
 protected:

  double maturity_age_;
  double reproduction_cooldown_;

  double ready_to_reproduce_at_;

  bool waiting_to_reproduce_;

  virtual double MaturityAge(const Mutable* const mutables) const;

  virtual void ResetReproductionClock();
};

class MaleReproductiveSystem : virtual public ReproductiveSystem {
 public:
  MaleReproductiveSystem(neat::Genome genome, Mutable mutables);

  bool IsMale() const override { return true; }
  void MateWithFemale();
};

struct GestatingEgg {
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
  bool IsPregnant() const override { return egg_.has_value(); };
  GestatingEgg GetEgg() const { return egg_.value(); }

  virtual void Update(double delta_time) override;
  void MateWithMale(const Creature* const father, const Creature* const mother);
  bool CanBirth() const;
  std::shared_ptr<Egg> GiveBirth(const std::pair<double, double>& coordinates);

 protected:
  std::optional<GestatingEgg> egg_;  // gestating egg
  double gestation_ratio_to_incubation_;
};

#endif  // REPRODUCTION_H
