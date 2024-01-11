#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <unordered_map>

#include "config.h"
#include "food.h"
#include "movable_entity.h"
#include "alive_entity.h"
#include "vision_system.h"
#include "digestive_system.h"
#include "mutable.h"

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
class Creature : virtual public MovableEntity, virtual public AliveEntity, virtual public VisionSystem, virtual public DigestiveSystem {
 public:
  Creature(neat::Genome genome, Mutable mutable_);

  void UpdateEnergy(double deltaTime);
  void Update(double deltaTime, double const kMapWidth, double const kMapHeight,
              std::vector<std::vector<std::vector<Entity *> > > &grid,
              double GridCellSize, double frictional_coefficient);

  void OnCollision(Entity &other_entity, double const kMapWidth,
                   double const kMapHeight) override;

  bool Fit();
  void Reproduced();

  void Grow(double energy);
  void Think(std::vector<std::vector<std::vector<Entity *> > > &grid,
             double GridCellSize, double deltaTime, double width, double height);

  bool Compatible(const Creature& other_creature);

 protected:
  bool fit_; /*!< Indicates whether the creature is fit in the evolutionary
                sense. */
  double reproduction_cooldown_; /*!< Cooldown period before the creature can
                                    reproduce again. */
  int think_count; /*! Keeps track so that creatures think every 5 loops */
};

std::vector<Food *> get_food_at_distance(
    std::vector<std::vector<std::vector<Entity *>>> &grid, int i_creature,
    int j_creature, int grid_distance);
#endif  // CREATURE_HPP
