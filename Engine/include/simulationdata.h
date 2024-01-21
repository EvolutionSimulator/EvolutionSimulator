#pragma once
#include <queue>
#include <unordered_map>
#include <vector>

#include "creature.h"
#include "egg.h"
#include "environment.h"
#include "food.h"

struct SimulationData {
 public:
  std::vector<int> GetCreatureCountOverTime() const;
  explicit SimulationData(myEnvironment::Environment& env)
      : environment_(env),
        creatures_(),
        food_entities_(),
        lastRecordedTime_(0.0) {
    InitializeFood();
    InitializeCreatures();
    InitializeGrid();
  }

  void InitializeCreatures();
  void InitializeFood();
  void InitializeGrid();

  void AddCreature(const Creature& entity);
  void RemoveCreature(const Creature& entity);

  void AddEgg(const Egg& egg);
  void HatchEggs();

  void GenerateMoreFood();
  void UpdateGrid();
  void ClearGrid();

  void CheckFoodCollisions();
  void CheckCreatureCollisions();
  void CheckCollisions();

  void ModifyAllCreatures(double delta_x, double delta_y);
  void UpdateAllCreatures(double deltaTime);

  myEnvironment::Environment GetEnvironment();
  void SetEnvironment(myEnvironment::Environment& environment);
  std::vector<std::vector<std::vector<Entity*>>> GetGrid();

  void ReproduceCreatures();
  void ReproduceTwoCreatures(Creature& creature1, Creature& creature2);

  std::vector<Creature> creatures_;
  std::vector<Food> food_entities_;
  std::vector<Egg> eggs_;
  std::queue<int>
      reproduce_;  // Stores indices of creatures that are ready to reproduce
  std::queue<int> new_reproduce_;  // Stores indices of creatures that are ready
                                   // to reproduce

  double world_time_ = 0;

 private:
  double lastRecordedTime_;
  myEnvironment::Environment& environment_;
  std::vector<std::vector<std::vector<Entity*>>> grid_;
  std::vector<int> creatureCountOverTime_;
};

std::vector<std::pair<int, int>> GetNeighbours(
    const int& num_rows, const int& num_cols, const std::pair<int, int>& center,
    const int& layer_number);
