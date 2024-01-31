#pragma once

#include "simulation/entity_grid.h"

class CollisionManager {
 public:
  CollisionManager();

  void CheckCollisions(EntityGrid& entity_grid);
};
