#ifndef CREATURE_H
#define CREATURE_H

#include <unordered_map>

#include "core/settings.h"
#include "entities/food.h"
#include "entities/movable_entity.h"
#include "neat/neural_network.h"
#include "entities/mutable.h"

/*!
 * @file creature.h
 * @brief Defines the Creature class and related functionalities in a NEAT-based
 * simulation environment.
 *
 * @details The Creature class, derived from MovableEntity, represents an agent
 * in the simulation. It includes attributes like health, energy, age, and a
 * neural network for decision-making. This class handles creature behaviors
 * such as consuming food, energy management, growth, and collision responses.
 */

/*!
 * @class Creature
 * @brief Represents an agent in a NEAT-based simulation environment, inheriting
 * from MovableEntity.
 *
 * @details This class models a creature with various attributes and behaviors
 * pivotal for a NEAT-based simulation. It encompasses aspects like health,
 * energy, age, and growth, and integrates a neural network for decision-making.
 *          The creature's behavior includes responding to collisions, searching
 * for and consuming food, and managing its energy and health. The class also
 * supports evolutionary features like reproduction and genetic inheritance.
 */
class Creature : public MovableEntity {
 public:
  Creature(neat::Genome genome, Mutable mutable_);

  void Dies();
  void Eats(double nutritional_value);
  void UpdateEnergy(double deltaTime);
  double GetEnergy() const;
  void SetEnergy(double energy);
  double GetMaxEnergy() const;
  void SetMaxEnergy(double max_energy);
  neat::Genome GetGenome();
  Mutable GetMutable();

  void Update(double deltaTime, double const kMapWidth, double const kMapHeight,
              std::vector<std::vector<std::vector<Entity *> > > &grid,
              double GridCellSize, double frictional_coefficient);


  double GetHealth() const;
  void SetHealth(double health);

  void OnCollision(Entity &other_entity, double const kMapWidth,
                   double const kMapHeight) override;

  bool Fit();
  void Reproduced();

  double GetMaxEnergy();
  void UpdateMaxEnergy();

  double GetAge() const;
  void SetAge(double age);

  void SetVision(double radius, double angle);
  double GetVisionRadius() const;
  double GetVisionAngle() const;

  void BalanceHealthEnergy();

  Food *GetClosestFood(std::vector<std::vector<std::vector<Entity *>>> &grid,
                       double GridCellSize) const;
  Food *GetClosestFoodInSight(
      std::vector<std::vector<std::vector<Entity *>>> &grid,
      double grid_cell_size) const;

  void Grow(double energy);

  void Think(std::vector<std::vector<std::vector<Entity *> > > &grid,
             double GridCellSize, double deltaTime);
  void ProcessVisionFood(
      std::vector<std::vector<std::vector<Entity *> > > &grid,
      double GridCellSize);

  int GetGeneration() const;
  void SetGeneration(int generation);

 protected:
  double max_energy_;
  double energy_; /*!< Stores the current energy level of the creature. */

  double health_; /*!< Represents the current health status of the creature. */

  double age_;    /*!< Tracks the age of the creature. */

  Mutable mutable_;

  neat::NeuralNetwork brain_; /*!< Neural network for processing environmental
                                 stimuli and decision making. */
  neat::Genome genome_;       /*!< Genetic makeup of the creature. */
  double distance_food_;      /*!< Distance to the nearest food source. */
  double orientation_food_;   /*!< Orientation relative to the nearest food
                                 source. */
  std::vector<double>
      neuron_data_; /*!< Neuron data used in the neural network. */
  bool fit_; /*!< Indicates whether the creature is fit in the evolutionary
                sense. */
  int generation_ = 0;           /*!< Generation count of the creature. */
  double reproduction_cooldown_; /*!< Cooldown period before the creature can
                                    reproduce again. */
  double vision_radius_; /*!< The radius within which the creature can detect
                            other entities. */
  double vision_angle_;  /*!< The angle of vision for the creature, representing
                            the field of view. */
};

std::vector<Food *> get_food_at_distance(
    std::vector<std::vector<std::vector<Entity *>>> &grid, int i_creature,
    int j_creature, int grid_distance);
#endif  // CREATURE_H
