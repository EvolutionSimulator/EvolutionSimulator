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
  void UpdateEnergy();
  double GetEnergy() const;
  void SetEnergy(double energy);
  neat::Genome GetGenome();
  void Update(double deltaTime, double const kMapWidth,
              double const kMapHeight);

  virtual void OnCollision(Food &food);
  virtual void OnCollision(Creature &creature);
  Food *GetClosestFood(
      std::unordered_map<int, std::unordered_map<int, std::vector<Entity *>>>
          &grid,
      double GridCellSize) const;

  void SetGrowthFactor(double growth_factor);
  double GetGrowthFactor();

  void SetMaxSize(double max_size);
  double GetMaxSize();

  void Grow(double energy);

  bool Fit();

private:
  double energy_, growth_factor_;
  double max_size_ = 10;
  neat::NeuralNetwork brain_;
  neat::Genome genome_;
};

std::vector<Food *> get_food_at_distance(
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity *>>>
        &grid,
    int i_creature, int j_creature, int grid_distance);

#endif // CREATURE_HPP
