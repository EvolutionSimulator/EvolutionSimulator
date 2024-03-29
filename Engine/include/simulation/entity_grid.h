#pragma once

#include "simulation/environment.h"
#include "simulation/simulation_data.h"

class EntityGrid {
 public:
  EntityGrid();

  void InitializeGrid(SimulationData &data, Environment &environment);

  void UpdateGrid(SimulationData &data, Environment &environment, double deltaTime);
  void ClearGrid();

  std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> GetGrid() const;

  const std::vector<std::shared_ptr<Entity>>& GetEntitiesAt(const int row, const int col) const;
  const std::vector<std::shared_ptr<Entity>>& GetEntitiesAt(const std::pair<int, int>& coords) const;

  const std::pair<int, int> GetGridSize() const;

  std::vector<std::pair<int, int>> GetNeighbours(const std::pair<int, int>& center, const int& layer_number);

 private:
  std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> grid_;
  int num_columns_, num_rows_;
};
