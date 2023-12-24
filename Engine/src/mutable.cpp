#include "mutable.h"

Mutable::Mutable()
  : energy_density_(0.5), energy_loss_(0.5),
    strafing_difficulty_(0.5), max_size_(10),
    baby_size_(2), max_force_(10), growth_factor_(5) {
  double complexity  = Complexity();
  maturity_age_ = complexity * (max_size_ - baby_size_) * 0.2;
  reproduction_cooldown_ = complexity * 0.5;
}

double Mutable::Complexity() {
  //values to tweak in order to achieve ideal conditions
  double complexity = (energy_density_*10
          + 5/energy_loss_ + max_force_ * 2
          + 5/growth_factor_) * baby_size_;
  return complexity;
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
