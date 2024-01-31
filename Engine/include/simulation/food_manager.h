#pragma once

#include "simulation/simulation_data.h"
#include "simulation/environment.h"

class FoodManager {
 public:
  FoodManager();

  void UpdateAllFood(SimulationData &data, Environment &environment, double deltaTime);
  void InitializeFood(SimulationData &data, Environment &environment);
  void GenerateMoreFood(SimulationData &data, Environment &environment, double deltaTime);
  void UpdateAllFood(SimulationData &data, double deltaTime);
};
