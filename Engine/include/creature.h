#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <vector>

#include "food.h"
#include "movable_entity.h"
#include "neat/neat-neural-network.h"

class Creature : public MovableEntity {
 public:
  Creature();

  void Dies();
  void Eats(double nutritional_value);
  void UpdateEnergy(const double energyToHealth, const double healthToEnergy);
  double GetEnergy() const;
  void SetEnergy(double energy);

  double GetHealth() const;
  void SetHealth(double health);

  void HealthToEnergy();
  void EnergyToHealth();

  virtual void OnCollision(Food& food);
  virtual void OnCollision(Creature& creature);

  std::vector<double> GetEnvironmentalData() const;
  void UpdateFromBrain(const std::vector<double>& input_values);

 protected:
  neat::NeuralNetwork brain_;
  double energy_;
  double health_;
};

#endif  // CREATURE_HPP
