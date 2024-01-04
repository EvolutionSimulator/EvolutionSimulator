#ifndef VISIONSYSTEM_H
#define VISIONSYSTEM_H

#include "alive_entity.h"
#include "food.h"

class VisionSystem : virtual public AliveEntity {
public:
  VisionSystem(neat::Genome genome, Mutable mutables);

  void SetVision(double radius, double angle);
  double GetVisionRadius() const;
  double GetVisionAngle() const;

  Food *GetClosestFood(std::vector<std::vector<std::vector<Entity *>>> &grid,
                       double GridCellSize) const;
  Food *GetClosestFoodInSight(
      std::vector<std::vector<std::vector<Entity *>>> &grid,
      double grid_cell_size) const;

  void ProcessVisionFood(std::vector<std::vector<std::vector<Entity *>>> &grid,
                         double grid_cell_size, double width, double height);
  int GetFoodID() const;
};

#endif // VISIONSYSTEM_H
