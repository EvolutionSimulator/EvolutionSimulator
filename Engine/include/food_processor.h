#pragma once

#include "simulation_data.h"

class FoodProcessor
{
 public:
  FoodProcessor();

  void InitializeFood(SimulationData& data, Environment& environment);
  void GenerateMoreFood(SimulationData& data, Environment& environment);
};

