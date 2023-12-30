#include "mutable.h"
#include "settings.h"
#include <random>

/*!
 * @brief Constructs a new Mutable object.
 *
 * @details Initializes the Mutable object with predefined values from the
 *          settings::physical_constraints namespace.
 */
Mutable::Mutable()
    : energy_density_(SETTINGS.physical_constraints.d_energy_density),
      energy_loss_(SETTINGS.physical_constraints.d_energy_loss),
      integrity_(SETTINGS.physical_constraints.d_integrity),
      strafing_difficulty_(SETTINGS.physical_constraints.d_strafing_difficulty),
      max_size_(SETTINGS.physical_constraints.d_max_size),
      baby_size_(SETTINGS.physical_constraints.d_baby_size),
      max_force_(SETTINGS.physical_constraints.d_max_force),
      growth_factor_(SETTINGS.physical_constraints.d_growth_factor) {
  UpdateReproduction();
}

/*!
 * @brief Updates the reproduction characteristics of the entity.
 * @details Calculates and sets the maturity age and reproduction cooldown
 * based on the entity's complexity and size.
 */
void Mutable::UpdateReproduction() {
  double complexity = Complexity();
  maturity_age_ = complexity * (1 + max_size_ - baby_size_) * 0.2;
  reproduction_cooldown_ = complexity * 0.5;
}

/*!
 * @brief Calculates the complexity of the entity.
 * @return The calculated complexity as a double.
 * @details The complexity is calculated based on various properties
 * like energy density, integrity, size, etc.
 */
double Mutable::Complexity() {
  // values to tweak in order to achieve ideal conditions
  double complexity =
      (energy_density_ * 10 + 5 / energy_loss_ + integrity_ * 20 +
       5 / (1 + strafing_difficulty_) + max_force_ * 2 + 5 / growth_factor_) *
      baby_size_ / 10;
  return complexity;
}

/*!
 * @brief Mutates various properties of the entity randomly.
 * @details Applies random mutations to properties like energy density,
 * integrity, and size within certain constraints.
 */
void Mutable::Mutate() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  // Energy Density
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_energy_density / 20);
    double delta = dis(gen);
    energy_density_ += delta;
    if (energy_density_ > SETTINGS.physical_constraints.max_energy_density) {
      energy_density_ = SETTINGS.physical_constraints.max_energy_density;
    } else if (energy_density_ < 0) {
      energy_density_ = 0;
    }
  }

  // Energy Loss
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_energy_loss / 20);
    double delta = dis(gen);
    energy_loss_ += delta;
    if (energy_loss_ < SETTINGS.physical_constraints.min_energy_loss) {
      energy_loss_ = SETTINGS.physical_constraints.min_energy_loss;
    }
  }

  // Integrity
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_energy_density / 20);
    double delta = dis(gen);
    integrity_ += delta;
    if (integrity_ < 0) {
      integrity_ = 0;
    }
  }

  // Strafing Difficulty
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_strafing_difficulty / 20);
    double delta = dis(gen);
    strafing_difficulty_ += delta;
    if (strafing_difficulty_ < 0) {
      strafing_difficulty_ = 0;
    }
  }

  // Max Size
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_max_size / 20);
    double delta = dis(gen);
    max_size_ += delta;
    if (max_size_ < SETTINGS.environment.min_creature_size) {
      max_size_ = SETTINGS.environment.min_creature_size;
    }
  }

  // Baby Size
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_baby_size / 20);
    double delta = dis(gen);
    baby_size_ += delta;
    if (baby_size_ < SETTINGS.environment.min_creature_size) {
      baby_size_ = SETTINGS.environment.min_creature_size;
    } else if (baby_size_ > max_size_) {
      baby_size_ = max_size_;
    }
  }

  // Max Force
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_max_force / 20);
    double delta = dis(gen);
    max_force_ += delta;
    if (max_force_ < 0) {
      max_force_ = 0;
    }
  }

  // Growth Factor
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_max_force / 20);
    double delta = dis(gen);
    growth_factor_ += delta;
    if (growth_factor_ < 0) {
      growth_factor_ = 0;
    }
  }

  // Vision Factor
  if (uniform(gen) < SETTINGS.physical_constraints.mutation_rate) {
    std::normal_distribution<> dis(
        0.0, SETTINGS.physical_constraints.d_vision_factor / 20);
    double delta = dis(gen);
    vision_factor_ += delta;
    if (vision_factor_ < 0.1) {
      vision_factor_ = 0.1;
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
double Mutable::GetReproductionCooldown() const {
  return reproduction_cooldown_;
}
double Mutable::GetMaturityAge() const { return maturity_age_; }

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
void Mutable::SetReproductionCooldown(double value) {
  reproduction_cooldown_ = value;
}
void Mutable::SetMaturityAge(double value) { maturity_age_ = value; }

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
  crossover.SetEnergyDensity(
      (2 * dominant.GetEnergyDensity() + recessive.GetEnergyDensity()) / 3);
  crossover.SetEnergyLoss(
      (2 * dominant.GetEnergyLoss() + recessive.GetEnergyLoss()) / 3);
  crossover.SetIntegrity(
      (2 * dominant.GetIntegrity() + recessive.GetIntegrity()) / 3);
  crossover.SetStrafingDifficulty((2 * dominant.GetStrafingDifficulty() +
                                   recessive.GetStrafingDifficulty()) /
                                  3);
  crossover.SetMaxSize((2 * dominant.GetMaxSize() + recessive.GetMaxSize()) /
                       3);
  crossover.SetBabySize((2 * dominant.GetBabySize() + recessive.GetBabySize()) /
                        3);
  crossover.SetMaxForce((2 * dominant.GetMaxForce() + recessive.GetMaxForce()) /
                        3);
  crossover.SetGrowthFactor(
      (2 * dominant.GetGrowthFactor() + recessive.GetGrowthFactor()) / 3);
  crossover.SetVisionFactor(
      (2 * dominant.GetVisionFactor() + recessive.GetVisionFactor()) / 3);
  crossover.UpdateReproduction();
  return crossover;
}
