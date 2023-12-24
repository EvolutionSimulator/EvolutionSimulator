#include "mutable.h"
#include "config.h"
#include <random>

Mutable::Mutable()
  : energy_density_(settings::physical_constraints::kDEnergyDensity),
    energy_loss_(settings::physical_constraints::kDEnergyLoss),
    strafing_difficulty_(settings::physical_constraints::kDStrafingDifficulty),
    max_size_(settings::physical_constraints::kDMaxSize),
    baby_size_(settings::physical_constraints::kDBabySize),
    max_force_(settings::physical_constraints::kDMaxForce),
    growth_factor_(settings::physical_constraints::kDGrowthFactor) {
  UpdateReproduction();
}

void Mutable::UpdateReproduction() {
  double complexity  = Complexity();
  maturity_age_ = complexity * (1 + max_size_ - baby_size_) * 0.2;
  reproduction_cooldown_ = complexity * 0.5;
}

double Mutable::Complexity() {
  //values to tweak in order to achieve ideal conditions
  double complexity = (energy_density_*10
                       + 5/energy_loss_
                       + 5/(1+strafing_difficulty_)
                       + max_force_ * 2
                       + 5/growth_factor_) * baby_size_;
  return complexity;
}

void Mutable::Mutate() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  //Energy Density
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDEnergyDensity/10);
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
            settings::physical_constraints::kDEnergyLoss/10);
    double delta = dis(gen);
    energy_loss_ += delta;
    if (energy_loss_ < settings::physical_constraints::kMinEnergyLoss) {
        energy_loss_ = settings::physical_constraints::kMinEnergyLoss;
    }
  }

  //Strafing Difficulty
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDStrafingDifficulty/10);
    double delta = dis(gen);
    strafing_difficulty_ += delta;
    if (strafing_difficulty_ < 0) {
        strafing_difficulty_ = 0;
    }
  }

  //Max Size
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDMaxSize/10);
    double delta = dis(gen);
    max_size_ += delta;
    if (max_size_ < settings::environment::kMinCreatureSize) {
        max_size_ = settings::environment::kMinCreatureSize;
    }
  }

  //Baby Size
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDBabySize/10);
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
            settings::physical_constraints::kDMaxForce/10);
    double delta = dis(gen);
    max_force_ += delta;
    if (max_force_ < 0) {
        max_force_ = 0;
    }
  }

  //Growth Factor
  if (uniform(gen) < settings::physical_constraints::kMutationRate){
    std::normal_distribution<> dis(0.0,
            settings::physical_constraints::kDMaxForce/10);
    double delta = dis(gen);
    growth_factor_ += delta;
    if (growth_factor_ < 0) {
        growth_factor_ = 0;
    }
  }
}

// Getters
double Mutable::GetEnergyDensity() const { return energy_density_; }
double Mutable::GetEnergyLoss() const { return energy_loss_; }
double Mutable::GetStrafingDifficulty() const { return strafing_difficulty_; }
double Mutable::GetMaxSize() const { return max_size_; }
double Mutable::GetBabySize() const { return baby_size_; }
double Mutable::GetMaxForce() const { return max_force_; }
double Mutable::GetGrowthFactor() const { return growth_factor_; }
double Mutable::GetReproductionCooldown() const { return reproduction_cooldown_; }
double Mutable::GetMaturityAge() const { return maturity_age_; }

// Setters
void Mutable::SetEnergyDensity(double value) { energy_density_ = value; }
void Mutable::SetEnergyLoss(double value) { energy_loss_ = value; }
void Mutable::SetStrafingDifficulty(double value) { strafing_difficulty_ = value; }
void Mutable::SetMaxSize(double value) { max_size_ = value; }
void Mutable::SetBabySize(double value) { baby_size_ = value; }
void Mutable::SetMaxForce(double value) { max_force_ = value; }
void Mutable::SetGrowthFactor(double value) { growth_factor_ = value; }
void Mutable::SetReproductionCooldown(double value) { reproduction_cooldown_ = value; }
void Mutable::SetMaturityAge(double value) { maturity_age_ = value; }
