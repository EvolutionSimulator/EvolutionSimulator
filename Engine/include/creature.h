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
  void UpdateEnergy(double deltaTime);
  double GetEnergy() const;
  void SetEnergy(double energy);
  neat::Genome GetGenome();
  void Update(double deltaTime, double const kMapWidth,
                double const kMapHeight,
                std::vector<std::vector<std::vector<Entity*> > > &grid,
                double GridCellSize);

  double GetHealth() const;
  void SetHealth(double health);

  void OnCollision(Entity &other_entity, double const kMapWidth,
                   double const kMapHeight) override;

  bool Fit();
  void Reproduced();

  double GetMaxEnergy() const;
  void SetMaxEnergy(double max_energy);

  double GetAge() const;
  void SetAge(double age);

  void BalanceHealthEnergy();


  Food *GetClosestFood(
      std::vector<std::vector<std::vector<Entity*> > > &grid,
      double GridCellSize) const;

  void SetGrowthFactor(double growth_factor);
  double GetGrowthFactor();
  void SetMaxSize(double max_size);
  double GetMaxSize();

  void Grow(double energy);
  void Think(std::vector<std::vector<std::vector<Entity*> > > &grid, double GridCellSize);
  void ProcessVisionFood(std::vector<std::vector<std::vector<Entity*> > > &grid, double GridCellSize);

  int GetGeneration() const;
  void SetGeneration(int generation);

protected:
  double energy_; /*!< Stores the current energy level of the creature. */
  double growth_factor_; /*!< Determines how the creature grows in relation to energy intake. */
  double health_; /*!< Represents the current health status of the creature. */
  double age_; /*!< Tracks the age of the creature. */
  double max_energy_ = 300; /*!< The maximum energy level the creature can attain. */
  double max_size_ = 10; /*!< The maximum size the creature can grow to. */
  neat::NeuralNetwork brain_; /*!< Neural network for processing environmental stimuli and decision making. */
  neat::Genome genome_; /*!< Genetic makeup of the creature. */
  double distance_food_; /*!< Distance to the nearest food source. */
  double orientation_food_; /*!< Orientation relative to the nearest food source. */
  std::vector<double> neuron_data_; /*!< Neuron data used in the neural network. */
  bool fit_; /*!< Indicates whether the creature is fit in the evolutionary sense. */
  int generation_ = 0; /*!< Generation count of the creature. */
  double reproduction_cooldown_; /*!< Cooldown period before the creature can reproduce again. */

};


std::vector<Food *> get_food_at_distance(
    std::vector<std::vector<std::vector<Entity*> > > &grid,
    int i_creature, int j_creature, int grid_distance);
#endif // CREATURE_HPP
