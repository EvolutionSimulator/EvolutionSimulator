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
      double grid_cell_size, Food::type type) const;

  void ProcessVisionFood(std::vector<std::vector<std::vector<Entity *>>> &grid,
                         double grid_cell_size, double width, double height);
  int GetFoodID() const;
  Food *GetClosestPlantInSight(std::vector<std::vector<std::vector<Entity *>>> &grid,
          double grid_cell_size) const;
  Food *GetClosestMeatInSight(std::vector<std::vector<std::vector<Entity *>>> &grid,
          double grid_cell_size) const;
  bool IsFoodInSight(Food *food);

protected:
  double distance_plant_;       /*!< Distance to the nearest plant source. */
  double distance_meat_;        /*!< Distance to the nearest meat source. */
  double orientation_plant_;   /*!< Orientation relative to the nearest plant
                                 source. */
  double plant_size_;          /*! Size of the closest plant*/
  double meat_size_;          /*! Size of the closest meat*/
  int closest_plant_id_;       /*! Id of the closest plant to show in the UI */
  int closest_meat_id_;      /*! Id of the closest meat to show in the UI */
  double orientation_meat_;   /*!< Orientation relative to the nearest meat
                                source. */
  double vision_radius_; /*!< The radius within which the creature can detect
                            other entities. */
  double vision_angle_;  /*!< The angle of vision for the creature, representing
                            the field of view. */
};

double GetRandomFloat(double min_value, double max_value);
#endif // VISIONSYSTEM_H
