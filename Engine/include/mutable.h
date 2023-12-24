#ifndef MUTABLE_H
#define MUTABLE_H


class Mutable
{
public:
  Mutable();
  Mutable Crossover(Mutable &other_mutable) const;
  void Mutate();
  double Distance(Mutable &other_mutable) const;
  double Complexity();
  void UpdateReproduction();


  // Getters
  double GetEnergyDensity() const;
  double GetEnergyLoss() const;
  double GetStrafingDifficulty() const;
  double GetMaxSize() const;
  double GetBabySize() const;
  double GetMaxForce() const;
  double GetGrowthFactor() const;
  double GetReproductionCooldown() const;
  double GetMaturityAge() const;

  // Setters
  void SetEnergyDensity(double value);
  void SetEnergyLoss(double value);
  void SetStrafingDifficulty(double value);
  void SetMaxSize(double value);
  void SetBabySize(double value);
  void SetMaxForce(double value);
  void SetGrowthFactor(double value);
  void SetReproductionCooldown(double value);
  void SetMaturityAge(double value);

private:
  double energy_density_;
  double energy_loss_;
  double strafing_difficulty_; //value in movable entity so we'll need to treat it a lil bit different
  double max_size_;
  double baby_size_;
  double max_force_;
  double growth_factor_;
  double reproduction_cooldown_;
  double maturity_age_;
};

#endif // MUTABLE_H
