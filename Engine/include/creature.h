#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <unordered_map>
#include <memory>

#include "food.h"
#include "neat/neat-neural-network.h"

#include "movable_entity.h"
#include "alive_entity.h"
#include "vision_system.h"
#include "digestive_system.h"
#include "reproduction.h"
#include "grabbing_entity.h"
#include "pheromones_system.h"
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
class Creature : virtual public MovableEntity,
                 virtual public AliveEntity,
                 virtual public VisionSystem,
                 virtual public PheromoneSystem,
                 virtual public DigestiveSystem,                 
                 virtual public MaleReproductiveSystem,
                 virtual public FemaleReproductiveSystem {
 public:
  Creature(neat::Genome genome, Mutable mutable_);
  virtual ~Creature() override {}

  void UpdateEnergy(double deltaTime);

  void UpdateMatingDesire();

  void Update(double deltaTime, double const kMapWidth, double const kMapHeight,
              std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
              double GridCellSize, double frictional_coefficient);

  void OnCollision(std::shared_ptr<Entity>other_entity, double const kMapWidth,
                   double const kMapHeight) override;

  void Grow(double energy);
  void Think(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
             double GridCellSize, double deltaTime, double width, double height);


  void Bite(std::shared_ptr<Creature> creature);
  bool Compatible(const std::shared_ptr<Creature> other_creature);

  void Grab(std::shared_ptr<Entity>entity);
  void ProcessVision(std::shared_ptr<Entity> entity, int start);



  std::shared_ptr<Creature> GetClosestEnemyInSight(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
      double grid_cell_size, double map_width, double map_heigth);

  bool GetMatingDesire() const;

 protected:
  int think_count_; /*! Keeps track so that creatures think every 5 loops */
  bool mating_desire_; /*! Indicates whether creature currently wants to mate <-
                        TO BE INTEGRATED INTO REPRODUCTIVE SYSTEM*/
  bool attack_; /*! Indicates whether creature currently wants to attack */

};
#endif  // CREATURE_HPP
