#ifndef MUTABLE_H
#define MUTABLE_H

class Mutable
{
public:
  Mutable();
  void Mutate();
  double Distance(Mutable &other_mutable) const;
  double Complexity();
  void UpdateReproduction();


  // Getters
  double GetEnergyDensity() const;
  double GetEnergyLoss() const;
  double GetIntegrity() const;
  double GetStrafingDifficulty() const;
  double GetMaxSize() const;
  double GetBabySize() const;
  double GetMaxForce() const;
  double GetGrowthFactor() const;
  double GetVisionFactor()  const;
  double GetReproductionCooldown() const;
  double GetMaturityAge() const;
  float GetColor() const;

  // Setters
  void SetEnergyDensity(double value);
  void SetEnergyLoss(double value);
  void SetIntegrity(double value);
  void SetStrafingDifficulty(double value);
  void SetMaxSize(double value);
  void SetBabySize(double value);
  void SetMaxForce(double value);
  void SetGrowthFactor(double value);
  void SetVisionFactor(double value);
  void SetReproductionCooldown(double value);
  void SetMaturityAge(double value);
  void SetColor(double hue);

  double CompatibilityBetweenMutables(const Mutable& other_mutable);
private:
  //Any values added here need to be included in the complexity, mutation, and crossover functions
  double energy_density_;
  double energy_loss_;
  double integrity_;
  double strafing_difficulty_; //value in movable entity so we'll need to treat it a lil bit different
  double max_size_;
  double baby_size_;
  double max_force_;
  double growth_factor_; /*!< Determines how the creature grows in relation to
                            energy intake. */
  double vision_factor_;
  double reproduction_cooldown_; /*!< Cooldown period before the creature can
                                    reproduce again. */
  double maturity_age_;
  float color_;
};

Mutable MutableCrossover(const Mutable& dominant, const Mutable& recessive);


#endif // MUTABLE_H
