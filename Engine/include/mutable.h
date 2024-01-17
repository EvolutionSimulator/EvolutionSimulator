#ifndef MUTABLE_H
#define MUTABLE_H

class Mutable {
 public:
  Mutable();
  void Mutate();
  double Distance(Mutable& other_mutable) const;
  double Complexity() const;

  // Getters
  double GetEnergyDensity() const;
  double GetEnergyLoss() const;
  double GetIntegrity() const;
  double GetStrafingDifficulty() const;
  double GetMaxSize() const;
  double GetBabySize() const;
  double GetMaxForce() const;
  double GetGrowthFactor() const;
  double GetVisionFactor() const;
  double GetGestationRatioToIncubation() const;
  float GetColor() const;
  double GetStomachCapacityFactor() const;
  double GetDiet() const;
  double GetGeneticStrength() const;
  double GetEatingSpeed() const;
  double GetPheromoneEmission() const;

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
  void SetGestationRatioToIncubation(double value);
  void SetColor(double hue);
  void SetStomachCapacityFactor(double value);
  void SetDiet(double value);
  void SetGeneticStrength(double value);
  void SetEatingSpeed(double value);
  void SetPheromoneEmission(double value);

  double CompatibilityBetweenMutables(const Mutable& other_mutable);

 private:
  // Any values added here need to be included in the complexity, mutation, and
  // crossover functions
  double energy_density_;
  double energy_loss_;
  double integrity_;
  double strafing_difficulty_;  // value in movable entity so we'll need to
                                // treat it a lil bit different
  double max_size_;
  double baby_size_;
  double max_force_;
  double growth_factor_; /*!< Determines how the creature grows in relation to
                            energy intake. */
  double vision_factor_;
  double gestation_ratio_to_incubation_;
  float color_;
  double stomach_capacity_factor_; /*!< Determines the stomach capacity in
                            relation to its size. */
  double diet_; /*!< Determines whether a creature is herbivore or carnivore */
  double genetic_strength_; /*! Determines bite stength */
<<<<<<< HEAD
  double eating_speed_;     /*! Determines eating and digestion cooldown */
=======
  double eating_speed_; /*! Determines eating and digestion cooldown */

  double pheromone_emission_;/*! Determines the rate of pheromone emission */
>>>>>>> 79742cc (Add basic structure for pheromones)
};

Mutable MutableCrossover(const Mutable& dominant, const Mutable& recessive);

#endif  // MUTABLE_H
