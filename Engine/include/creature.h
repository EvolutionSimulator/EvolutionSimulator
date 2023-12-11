#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "config.h"
#include "food.h"
#include "movable_entity.h"
#include "neat/neat-neural-network.h"

#include <unordered_map>

class Creature : public MovableEntity {
public:
  Creature(neat::Genome genome);

  void Dies();
  void Eats(double nutritional_value);
  void UpdateEnergy(const double energyToHealth, const double healthToEnergy);
  double GetEnergy() const;
  void SetEnergy(double energy);
  neat::Genome GetGenome();
  void Update(double deltaTime, double const kMapWidth,
              double const kMapHeight);

  double GetHealth() const;
  void SetHealth(double health);

  void HealthToEnergy();
  void EnergyToHealth();

  void OnCollision(Entity& other_entity) override;

  bool Fit();

  Food *GetClosestFood(
      std::vector<std::vector<std::vector<Entity*> > > &grid,
      double GridCellSize) const;

  void SetGrowthFactor(double growth_factor);
  double GetGrowthFactor();
  void SetMaxSize(double max_size);
  double GetMaxSize();

  void Grow(double energy);

  void Think(std::unordered_map<int, std::unordered_map<int, std::vector<Entity*> > > &grid, double GridCellSize);
  void ProcessVisionFood(std::unordered_map<int, std::unordered_map<int, std::vector<Entity*> > > &grid, double GridCellSize);



protected:
  double energy_, growth_factor_, health_;
  double max_size_ = 10;
  neat::NeuralNetwork brain_;
  neat::Genome genome_;
  double distance_food_;
  double orientation_food_;
};


std::vector<Food *> get_food_at_distance(
    std::vector<std::vector<std::vector<Entity*> > > &grid,
    int i_creature, int j_creature, int grid_distance);
#endif // CREATURE_HPP
