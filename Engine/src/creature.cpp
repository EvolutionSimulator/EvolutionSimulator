#include "creature.h"

#include <iostream>

#include "config.h"
#include "neat/neat-genome.h"

Creature::Creature()
    : MovableEntity(),
      health_(100),
      energy_(100),
      brain_(neat::Genome(CreatureParameters::kCreatureInputCount,
                          CreatureParameters::kCreatureOutputCount)) {}

double Creature::GetEnergy() const { return energy_; }

void Creature::SetEnergy(double energy) {
  if (energy > 100) {
    energy_ = 100;
  } else {
    energy_ = energy;
  }
}

void Creature::UpdateEnergy(const double energyToHealth,
                            const double healthToEnergy) {
  SetEnergy(GetEnergy() -
            (GetVelocityForward() + GetRotationalVelocity()) * GetSize());

  if (GetEnergy() <= healthToEnergy) {
    HealthToEnergy();
  } else if (GetEnergy() >= energyToHealth) {
    EnergyToHealth();
  }

  if (GetHealth() <= 0) {
    Dies();
  }
}

void Creature::HealthToEnergy() {
  if (GetEnergy() < 0) {
    SetHealth(GetHealth() - GetEnergy() - 5);
    SetEnergy(5);
  }
  SetEnergy(GetEnergy() + 5);
  SetHealth(GetHealth() - 5);
}

void Creature::EnergyToHealth() {
  SetEnergy(GetEnergy() - 5);
  SetHealth(GetHealth() + 5);
}

double Creature::GetHealth() const { return health_; }

void Creature::SetHealth(double health) {
  if (health > 100) {
    health_ = 100;
  } else {
    health_ = health;
  }
}

void Creature::Dies() { SetState(Dead); }

void Creature::Eats(double nutritional_value) {
  SetEnergy(GetEnergy() + nutritional_value);
  if (GetEnergy() > 100) {
    EnergyToHealth();
  }
}

void Creature::OnCollision(Food& food) {
  if (food.GetState() == Entity::Alive) {
    Eats(food.GetNutritionalValue());
    food.Eat();
  }
}

void Creature::OnCollision(Creature& creature) {}

std::vector<double> Creature::GetEnvironmentalData()
    const {  // needs to be modified with the actual data
  std::vector<double> data = {1, 1};
  return data;
}

void Creature::UpdateFromBrain(const std::vector<double>& input_values) {
  std::vector<double> output_values = brain_.Activate(input_values);
  SetVelocityForward(CreatureParameters::kForwardVelocityMultiplier *
                     output_values[0]);
  SetRotationalVelocity(CreatureParameters::kRotationalVelocityMultiplier *
                        output_values[1]);
}
