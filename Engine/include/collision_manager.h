#pragma once

#include "entity_grid.h"

class CollisionManager
{
 public:
  CollisionManager();

  void CheckCollisions(EntityGrid& entity_grid);
};

