#pragma once
#include <queue>
#include <unordered_map>
#include <vector>

#include "creature.h"
#include "environment.h"
#include "food.h"


struct SimulationData {
 public:
  explicit SimulationData(Environment& env)
      : environment_(env), creatures_(), food_entities_(), lastRecordedTime_(0.0){
    // InitializeFood();
    // InitializeCreatures();
    // InitializeGrid();
  }

  Environment GetEnvironment();
  void SetEnvironment(Environment& environment);

  std::vector<std::vector<std::vector<Entity*>>> GetGrid();

  void UpdateStatistics();

  std::vector<int> GetCreatureCountOverTime() const;
  std::vector<double> GetCreatureSizeOverTime() const;
  std::vector<double> GetCreatureEnergyOverTime() const;
  std::vector<double> GetCreatureVelocityOverTime() const;

  std::vector<Creature> creatures_;
  std::vector<Food> food_entities_;
  std::queue<Creature> reproduce_;
  std::queue<Creature> new_reproduce_;

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
