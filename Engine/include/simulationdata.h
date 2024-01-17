#pragma once
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>

#include "creature.h"
#include "egg.h"
#include "environment.h"
#include "food.h"
#include "pheromone.h"

struct SimulationData {
 public:
  explicit SimulationData(Environment& env)
      : environment_(env),
        creatures_(),
        food_entities_(),
        lastRecordedTime_(0.0) {
    // InitializeFood();
    // InitializeCreatures();
    // InitializeGrid();
  }

  Environment GetEnvironment();
  void SetEnvironment(Environment& environment);

  std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> GetGrid();


  void UpdateStatistics();

  std::vector<int> GetCreatureCountOverTime() const;
  std::vector<double> GetCreatureSizeOverTime() const;
  std::vector<double> GetCreatureEnergyOverTime() const;
  std::vector<double> GetCreatureVelocityOverTime() const;

  std::vector<std::shared_ptr<Creature>> creatures_;
  std::vector<std::shared_ptr<Food>> food_entities_;
  std::vector<std::shared_ptr<Egg>> eggs_;
  std::vector<std::shared_ptr<Pheromone>> pheromones_;
  std::queue<std::shared_ptr<Creature>> reproduce_;
  std::queue<std::shared_ptr<Creature>> new_reproduce_;

  double world_time_ = 0;

 private:
  double lastRecordedTime_;
  Environment& environment_;

  std::vector<int> creatureCountOverTime_;
  std::vector<double> creatureSizeOverTime_;
  std::vector<double> creatureEnergyOverTime_;
  std::vector<double> creatureVelocityOverTime_;
};

std::vector<std::pair<int, int>> GetNeighbours(
    const int& num_rows, const int& num_cols, const std::pair<int, int>& center,
    const int& layer_number);
