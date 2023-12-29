#pragma once

#include "simulation_data.h"

class FoodManager
{
 public:
  FoodManager();

  void InitializeFood(SimulationData& data, Environment& environment);
  void GenerateMoreFood(SimulationData& data, Environment& environment);
};

