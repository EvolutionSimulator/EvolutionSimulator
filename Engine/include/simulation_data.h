#pragma once
#include <queue>
#include <unordered_map>
#include <vector>

#include "creature.h"
#include "environment.h"
#include "food.h"

struct SimulationData {
public:
  SimulationData() : creatures_(), food_entities_() {}

  std::vector<Creature> creatures_;
  std::vector<Food> food_entities_;
  std::queue<Creature> reproduce_;

  double world_time_ = 0;

private:
  std::vector<std::vector<std::vector<Entity *>>> grid_;
};
