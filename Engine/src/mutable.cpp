#include "mutable.h"

#include <algorithm>
#include <iostream>
#include "random.h"

#include "math.h"
#include "mathlib.h"
#include "settings.h"

/*!
 * @brief Constructs a new Mutable object.
 *
 * @details Initializes the Mutable object with predefined values from the
 *          SETTINGS.physical_constraints namespace.
 */
Mutable::Mutable()
  : energy_density_(SETTINGS.physical_constraints.d_energy_density),
    energy_loss_(SETTINGS.physical_constraints.d_energy_loss),
    integrity_(SETTINGS.physical_constraints.d_integrity),
    strafing_difficulty_(SETTINGS.physical_constraints.d_strafing_difficulty),
    max_size_(SETTINGS.physical_constraints.d_max_size),
    baby_size_(SETTINGS.physical_constraints.d_baby_size),
    max_force_(SETTINGS.physical_constraints.d_max_force),
    growth_factor_(SETTINGS.physical_constraints.d_growth_factor),
    vision_factor_(SETTINGS.physical_constraints.d_vision_factor),
    color_(Random::Double(0, 1)),
    stomach_capacity_factor_(SETTINGS.physical_constraints.d_stomach_capacity),
    diet_(SETTINGS.physical_constraints.d_diet),
    genetic_strength_(SETTINGS.physical_constraints.d_genetic_strength),
    eating_speed_(SETTINGS.physical_constraints.d_eating_cooldown),
    pheromone_emission_(SETTINGS.physical_constraints.d_pheromone_emission),
    gestation_ratio_to_incubation_(SETTINGS.physical_constraints.
                                   d_gestation_ratio_to_incubation)
{

}

/*!
 * @brief Calculates the complexity of the entity.
 * @return The calculated complexity as a double.
 * @details The complexity is calculated based on various properties
 * like energy density, integrity, size, etc.
 */
double Mutable::Complexity() const {
  //values to tweak in order to achieve ideal conditions
  double complexity = (energy_density_*10
                       + 5/energy_loss_
                       + integrity_ * 20
                       + 5/(1+strafing_difficulty_)
                       + max_force_ * 2
                       + 5/growth_factor_
                       + stomach_capacity_factor_
                       + eating_speed_
                       + genetic_strength_
                       + pheromone_emission_) * baby_size_/12;
  return complexity;
}

/*!
 * @brief Mutates various properties of the entity randomly.
 * @details Applies random mutations to properties like energy density,
 * integrity, and size within certain constraints.
 */
void Mutable::Mutate() {

  // Energy Density
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_energy_density / 20);
    energy_density_ += delta;
    energy_density_ = mathlib::bound(energy_density_, 0.01,
                                     SETTINGS.physical_constraints.max_energy_density);
  }

  // Energy Loss
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_energy_loss / 20);    
    energy_loss_ += delta;
    if (energy_loss_ < SETTINGS.physical_constraints.min_energy_loss) {
      energy_loss_ = SETTINGS.physical_constraints.min_energy_loss;
    }
  }

  // Integrity
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_energy_density / 20);    
    integrity_ += delta;
    if (integrity_ < 0.01) {
      integrity_ = 0.01;
    }
  }

  // Strafing Difficulty
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_strafing_difficulty / 20);   
    strafing_difficulty_ += delta;
    if (strafing_difficulty_ < 0.01) {
      strafing_difficulty_ = 0.01;
    }
  }

  // Max Size
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_max_size / 20);    
    max_size_ += delta;
    if (max_size_ < SETTINGS.environment.min_creature_size) {
      max_size_ = SETTINGS.environment.min_creature_size;
    }
  }

  // Baby Size
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_baby_size / 20);    
    baby_size_ += delta;
    baby_size_ = mathlib::bound(baby_size_, SETTINGS.environment.min_creature_size,
                                max_size_);
  }

  // Max Force
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_max_force / 20);    
    max_force_ += delta;
    if (max_force_ < 0.01) {
      max_force_ = 0.01;
    }
  }

  // Growth Factor
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_max_force / 20);    
    growth_factor_ += delta;
    if (growth_factor_ < 0.01) {
      growth_factor_ = 0.01;
    }
  }

  // Vision Factor
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_vision_factor / 20);    
    vision_factor_ += delta;
    if (SETTINGS.physical_constraints.vision_ar_ratio / vision_factor_ >
        2 * M_PI) {
      vision_factor_ =
          SETTINGS.physical_constraints.vision_ar_ratio / (2 * M_PI);
    }
  }

  // Gestation Ratio To Incubation
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0,
        SETTINGS.physical_constraints.d_gestation_ratio_to_incubation / 20);    
    gestation_ratio_to_incubation_ += delta;
    gestation_ratio_to_incubation_ =
        mathlib::bound(gestation_ratio_to_incubation_, 0.01, 0.99);
  }

  // Color
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.color_mutation_factor);
    float float_delta = delta; //as shaders use floats
    color_ += float_delta;
    if (color_ > 1) {
      color_ -= 1;
    } else if (color_ < 0) {
      color_ += 1;
    }
  }

  // Stomach capacity
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_stomach_capacity / 20);
    stomach_capacity_factor_ += delta;
    stomach_capacity_factor_ = mathlib::bound(stomach_capacity_factor_, 0.01, 1);
  }

  // Diet
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(0.0,
                                   SETTINGS.physical_constraints.d_diet / 10);
    diet_ += delta;
    diet_ = mathlib::bound(diet_, 0.1, 0.9);
  }

  // Genetic Strength
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate) {
    double delta = Random::Normal(
        0.0, SETTINGS.physical_constraints.d_genetic_strength / 10);
    genetic_strength_ += delta;
    genetic_strength_ = mathlib::bound(genetic_strength_, 0.2, 1.2);
  }

  //Eating Speed
  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate){
    double delta = Random::Normal(0.0,
                                   SETTINGS.physical_constraints.d_eating_speed/10);
    eating_speed_ += delta;
    if (eating_speed_  < 0.2) {
        eating_speed_ = 0.2;
    }
    if (eating_speed_ > 1.2) {
        eating_speed_ = 1.2;
    }
  }

  if (Random::Double(0.0, 1.0) < SETTINGS.physical_constraints.mutation_rate){
    double delta = Random::Normal(0.0,
                                   SETTINGS.physical_constraints.d_pheromone_emission/10);
    pheromone_emission_ += delta;
    if (pheromone_emission_  < 0) {
        pheromone_emission_ = 0;
    }
    if (pheromone_emission_ > 1) {
        pheromone_emission_ = 1;
    }
  }
}

// Getters
double Mutable::GetEnergyDensity() const { return energy_density_; }
double Mutable::GetEnergyLoss() const { return energy_loss_; }
double Mutable::GetIntegrity() const { return integrity_; }
double Mutable::GetStrafingDifficulty() const { return strafing_difficulty_; }
double Mutable::GetMaxSize() const { return max_size_; }
double Mutable::GetBabySize() const { return baby_size_; }
double Mutable::GetMaxForce() const { return max_force_; }
double Mutable::GetGrowthFactor() const { return growth_factor_; }
double Mutable::GetVisionFactor() const { return vision_factor_; }
double Mutable::GetGestationRatioToIncubation() const {
  return gestation_ratio_to_incubation_;
}
float Mutable::GetColor() const { return color_; }
double Mutable::GetStomachCapacityFactor() const {
  return stomach_capacity_factor_;
}
double Mutable::GetDiet() const { return diet_; };
double Mutable::GetGeneticStrength() const { return genetic_strength_; };
double Mutable::GetEatingSpeed() const { return eating_speed_; };
double Mutable::GetPheromoneEmission() const {return pheromone_emission_;}

// Setters
void Mutable::SetEnergyDensity(double value) { energy_density_ = value; }
void Mutable::SetEnergyLoss(double value) { energy_loss_ = value; }
void Mutable::SetIntegrity(double value) { integrity_ = value; }
void Mutable::SetStrafingDifficulty(double value) {
  strafing_difficulty_ = value;
}
void Mutable::SetMaxSize(double value) { max_size_ = value; }
void Mutable::SetBabySize(double value) { baby_size_ = value; }
void Mutable::SetMaxForce(double value) { max_force_ = value; }
void Mutable::SetGrowthFactor(double value) { growth_factor_ = value; }
void Mutable::SetVisionFactor(double value) { vision_factor_ = value; }
void Mutable::SetGestationRatioToIncubation(double value) {
  gestation_ratio_to_incubation_ = value;
}
void Mutable::SetColor(double hue) { color_ = hue; }
void Mutable::SetStomachCapacityFactor(double value) {
  stomach_capacity_factor_ = value;
}
void Mutable::SetDiet(double value) { diet_ = value; };
void Mutable::SetGeneticStrength(double value) { genetic_strength_ = value; };
void Mutable::SetEatingSpeed(double value) { eating_speed_ = value; };

void Mutable::SetPheromoneEmission(double value) {pheromone_emission_ = value; }


/*!
 * @brief Creates a new Mutable entity as a crossover of two existing entities.
 * @param dominant The Mutable entity considered as dominant in the crossover.
 * @param recessive The Mutable entity considered as recessive in the crossover.
 * @return A new Mutable entity as a result of the crossover.
 * @details The crossover combines properties of two entities, favoring the
 * dominant one.
 */
Mutable MutableCrossover(const Mutable &dominant, const Mutable &recessive) {
  Mutable crossover;
  crossover.SetEnergyDensity((2*dominant.GetEnergyDensity()
                              + recessive.GetEnergyDensity())/3);
  crossover.SetEnergyLoss((2*dominant.GetEnergyLoss() +
                           recessive.GetEnergyLoss() )/3);
  crossover.SetIntegrity((2*dominant.GetIntegrity() +
                          recessive.GetIntegrity() )/3);
  crossover.SetStrafingDifficulty((2*dominant.GetStrafingDifficulty() +
                                   recessive.GetStrafingDifficulty() )/3);
  crossover.SetMaxSize((2*dominant.GetMaxSize() +
                        recessive.GetMaxSize() )/3);
  crossover.SetBabySize((2*dominant.GetBabySize() +
                         recessive.GetBabySize() )/3);
  crossover.SetMaxForce((2*dominant.GetMaxForce() +
                         recessive.GetMaxForce() )/3);
  crossover.SetGrowthFactor((2*dominant.GetGrowthFactor() +
                             recessive.GetGrowthFactor() )/3);
  crossover.SetVisionFactor((2*dominant.GetVisionFactor() +
                             recessive.GetVisionFactor() )/3);
  crossover.SetColor((2*dominant.GetColor() +
                      recessive.GetColor())/3);
  crossover.SetStomachCapacityFactor((2*dominant.GetStomachCapacityFactor() +
                                      recessive.GetStomachCapacityFactor() )/3);
  crossover.SetDiet((2*dominant.GetDiet() +
                                      recessive.GetDiet() )/3);
  crossover.SetGeneticStrength((2*dominant.GetGeneticStrength() +
                     recessive.GetGeneticStrength() )/3);
  crossover.SetEatingSpeed((2*dominant.GetEatingSpeed() +
                                recessive.GetEatingSpeed() )/3);
  crossover.SetGestationRatioToIncubation((2 * dominant.GetGestationRatioToIncubation() +
                                           recessive.GetGestationRatioToIncubation()) /3);
  crossover.SetPheromoneEmission((2*dominant.GetPheromoneEmission() +
                                 recessive.GetPheromoneEmission())/3);
  return crossover;
}

/*!
 * @brief Calculates the compatibility distance between two `Mutable` objects.
 *
 * @details This function computes the compatibility distance between the
 * current `Mutable` object and another `Mutable` object. The distance is
 * calculated by summing the absolute differences of various mutable attributes
 * (like energy density, energy loss, integrity, etc.) between the two objects,
 * each normalized by a respective physical constraint constant. The total
 * distance is then scaled by a compatibility factor. A lower distance value
 * indicates higher compatibility.
 *
 * The mutable attributes considered are:
 * - Energy Density
 * - Energy Loss
 * - Integrity
 * - Strafing Difficulty
 * - Max Size
 * - Baby Size
 * - Max Force
 * - Growth Factor
 * - Vision Factor
 *
 * @param other_mutable A constant reference to another `Mutable` object to
 * compare with.
 * @return double The calculated compatibility distance. A lower value indicates
 * higher compatibility between the two `Mutable` objects.
 */
double Mutable::CompatibilityBetweenMutables(
    const Mutable &other_mutable) const {
  double distance = 0;
  // Energy Density
  distance +=
      fabs(other_mutable.GetEnergyDensity() - this->GetEnergyDensity()) /
      SETTINGS.physical_constraints.d_energy_density;

  // Energy Loss
  distance += fabs(other_mutable.GetEnergyLoss() - this->GetEnergyLoss()) /
              SETTINGS.physical_constraints.d_energy_loss;

  // Integrity
  distance += fabs(other_mutable.GetIntegrity() - this->GetIntegrity()) /
              SETTINGS.physical_constraints.d_integrity;

  // Strafing Difficulty
  distance += fabs(other_mutable.GetStrafingDifficulty() -
                   this->GetStrafingDifficulty()) /
              SETTINGS.physical_constraints.d_strafing_difficulty;

  // Max Size
  distance += fabs(other_mutable.GetMaxSize() - this->GetMaxSize()) /
              SETTINGS.physical_constraints.d_max_size;

  // Baby Size
  distance += fabs(other_mutable.GetBabySize() - this->GetBabySize()) /
              SETTINGS.physical_constraints.d_baby_size;

  // Max Force
  distance += fabs(other_mutable.GetMaxForce() - this->GetMaxForce()) /
              SETTINGS.physical_constraints.d_max_force;

  // Growth Factor
  distance += fabs(other_mutable.GetGrowthFactor() - this->GetGrowthFactor()) /
              SETTINGS.physical_constraints.d_growth_factor;

  // Vision Factor
  distance += fabs(other_mutable.GetVisionFactor() - this->GetVisionFactor()) /
              SETTINGS.physical_constraints.d_vision_factor;

  // Gestation Ratio To Incubation
  distance += fabs(other_mutable.GetGestationRatioToIncubation() -
                   this->GetGestationRatioToIncubation()) /
              SETTINGS.physical_constraints.d_gestation_ratio_to_incubation;

  // Color
  distance += fabs(other_mutable.GetColor() - this->GetColor());

  // Stomach Capacity Factor
  distance += fabs(other_mutable.GetStomachCapacityFactor() - this->GetStomachCapacityFactor())
              / SETTINGS.physical_constraints.d_stomach_capacity;

  // Diet
  distance += fabs(other_mutable.GetDiet() - this->GetDiet())
              / SETTINGS.physical_constraints.d_diet;

  // Genetic Strength
  distance += fabs(other_mutable.GetGeneticStrength() - this->GetGeneticStrength())
              / SETTINGS.physical_constraints.d_genetic_strength;

  // Eating Speed
  distance += fabs(other_mutable.GetEatingSpeed() - this->GetEatingSpeed())
              / SETTINGS.physical_constraints.d_eating_speed;

  // Pheromone Emission
  distance += fabs(other_mutable.GetPheromoneEmission() - this->GetPheromoneEmission())
              / SETTINGS.physical_constraints.d_pheromone_emission;

  return distance * SETTINGS.compatibility.mutables_compatibility;
}
