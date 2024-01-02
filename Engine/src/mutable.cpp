#include "mutable.h"
#include "config.h"
#include <random>
#include <algorithm>

/*!
 * @brief Constructs a new Mutable object.
 *
 * @details Initializes the Mutable object with predefined values from the
 *          settings::physical_constraints namespace.
 */
Mutable::Mutable()
  : energy_density_(settings::physical_constraints::kDEnergyDensity),
    energy_loss_(settings::physical_constraints::kDEnergyLoss),
    integrity_(settings::physical_constraints::kDIntegrity),
    strafing_difficulty_(settings::physical_constraints::kDStrafingDifficulty),
    max_size_(settings::physical_constraints::kDMaxSize),
    baby_size_(settings::physical_constraints::kDBabySize),
    max_force_(settings::physical_constraints::kDMaxForce),
    growth_factor_(settings::physical_constraints::kDGrowthFactor),
    color_(0.5f){
  UpdateReproduction();
}

/*!
 * @brief Updates the reproduction characteristics of the entity.
 * @details Calculates and sets the maturity age and reproduction cooldown
 * based on the entity's complexity and size.
 */
void Mutable::UpdateReproduction() {
  double complexity  = Complexity();
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
  //values to tweak in order to achieve ideal conditions
  double complexity = (energy_density_*10
                       + 5/energy_loss_
                       + integrity_ * 20
                       + 5/(1+strafing_difficulty_)
                       + max_force_ * 2
                       + 5/growth_factor_) * baby_size_/10;
  return complexity;
}

/*!
 * @brief Mutates various properties of the entity randomly.
 * @details Applies random mutations to properties like energy density, integrity,
 * and size within certain constraints.
 */
void Mutable::Mutate() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  //Energy Density
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDEnergyDensity/20);
    double delta = dis(gen);
    energy_density_ += delta;
    if (energy_density_ > settings::physical_constraints::kMaxEnergyDensity) {
        energy_density_ = settings::physical_constraints::kMaxEnergyDensity;
    } else if (energy_density_ < 0) {
        energy_density_ = 0;
    }
  }

  //Energy Loss
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDEnergyLoss/20);
    double delta = dis(gen);
    energy_loss_ += delta;
    if (energy_loss_ < settings::physical_constraints::kMinEnergyLoss) {
        energy_loss_ = settings::physical_constraints::kMinEnergyLoss;
    }
  }

  //Integrity
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDEnergyDensity/20);
    double delta = dis(gen);
    integrity_ += delta;
    if (integrity_ < 0) {
        integrity_ = 0;
    }
  }


  //Strafing Difficulty
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDStrafingDifficulty/20);
    double delta = dis(gen);
    strafing_difficulty_ += delta;
    if (strafing_difficulty_ < 0) {
        strafing_difficulty_ = 0;
    }
  }

  //Max Size
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDMaxSize/20);
    double delta = dis(gen);
    max_size_ += delta;
    if (max_size_ < settings::environment::kMinCreatureSize) {
        max_size_ = settings::environment::kMinCreatureSize;
    }
  }

  //Baby Size
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDBabySize/20);
    double delta = dis(gen);
    baby_size_ += delta;
    if (baby_size_ < settings::environment::kMinCreatureSize) {
        baby_size_ = settings::environment::kMinCreatureSize;
    } else if (baby_size_ > max_size_) {
        baby_size_ = max_size_;
    }
  }

  //Max Force
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDMaxForce/20);
    double delta = dis(gen);
    max_force_ += delta;
    if (max_force_ < 0) {
        max_force_ = 0;
    }
  }

  //Growth Factor
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDMaxForce/20);
    double delta = dis(gen);
    growth_factor_ += delta;
    if (growth_factor_ < 0) {
        growth_factor_ = 0;
    }
  }

  //Vision Factor
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDVisionFactor/20);
    double delta = dis(gen);
    vision_factor_ += delta;
    if (settings::physical_constraints::kVisionARratio/vision_factor_ > 2* M_PI) {
        vision_factor_ = settings::physical_constraints::kVisionARratio/(2* M_PI);
    }
  }

  if (uniform(gen) < settings::physical_constraints::kMutationRate){
      std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kColorMutationFactor);
      float delta = dis(gen);
      color_ += delta;
      if (color_ > 1){
          color_ -= 1;
      } else if (color_ < 0){
          color_ += 1;
      }
  }

  //Color
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
double Mutable::GetReproductionCooldown() const { return reproduction_cooldown_; }
double Mutable::GetMaturityAge() const { return maturity_age_; }
float Mutable::GetColor() const { return color_; }

// Setters
void Mutable::SetEnergyDensity(double value) { energy_density_ = value; }
void Mutable::SetEnergyLoss(double value) { energy_loss_ = value; }
void Mutable::SetIntegrity(double value) { integrity_ = value; }
void Mutable::SetStrafingDifficulty(double value) { strafing_difficulty_ = value; }
void Mutable::SetMaxSize(double value) { max_size_ = value; }
void Mutable::SetBabySize(double value) { baby_size_ = value; }
void Mutable::SetMaxForce(double value) { max_force_ = value; }
void Mutable::SetGrowthFactor(double value) { growth_factor_ = value; }
void Mutable::SetVisionFactor(double value) { vision_factor_ = value; }
void Mutable::SetReproductionCooldown(double value) { reproduction_cooldown_ = value; }
void Mutable::SetMaturityAge(double value) { maturity_age_ = value; }
void Mutable::SetColor(double hue) { color_ = hue; }

/*!
 * @brief Creates a new Mutable entity as a crossover of two existing entities.
 * @param dominant The Mutable entity considered as dominant in the crossover.
 * @param recessive The Mutable entity considered as recessive in the crossover.
 * @return A new Mutable entity as a result of the crossover.
 * @details The crossover combines properties of two entities, favoring the dominant one.
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
  crossover.UpdateReproduction();
  return crossover;
}
