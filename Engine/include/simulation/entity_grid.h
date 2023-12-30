#pragma once

#include <functional>
#include "core/data_accessor.h"
#include "entities/entity.h"
#include "simulation/simulation_data.h"
#include <vector>

class EntityGrid
{
 public:
  EntityGrid();

  void RefreshGrid(SimulationData& data, Environment& environment);
  void ClearGrid();

  const std::vector<std::vector<std::vector<Entity*>>>& GetGrid() const;

  const std::vector<Entity*>& GetEntitiesAt(const int row, const int col) const;
  const std::vector<Entity*>& GetEntitiesAt(const std::pair<int, int>& coords) const;

  const std::pair<int, int> GetGridSize() const;

  std::vector<std::pair<int, int>> GetNeighbours(const std::pair<int, int>& center, const int& layer_number);
 private:
  std::vector<std::vector<std::vector<Entity*>>> grid_;
  int num_columns_, num_rows_;
};

