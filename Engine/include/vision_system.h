#ifndef VISIONSYSTEM_H
#define VISIONSYSTEM_H

#include <memory>

#include "alive_entity.h"
#include "food.h"

class VisionSystem : virtual public AliveEntity {
public:
  VisionSystem(neat::Genome genome, Mutable mutables);
  virtual ~VisionSystem() override {}

  void SetVision(double radius, double angle);
  double GetVisionRadius() const;
  double GetVisionAngle() const;

  std::shared_ptr<Food> GetClosestFood(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                       double GridCellSize) const;
  std::shared_ptr<Food> GetClosestFoodInSight(
      std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
      double grid_cell_size, Food::type type) const;

  void ProcessVisionFood(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                         double grid_cell_size, double width, double height);
  std::shared_ptr<Food> GetFoodID() const;
  std::shared_ptr<Food> GetClosestPlantInSight(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
          double grid_cell_size) const;
  std::shared_ptr<Food> GetClosestMeatInSight(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
          double grid_cell_size) const;
  bool IsInSight(std::shared_ptr<Entity> entity);

protected:
  double distance_plant_;       /*!< Distance to the nearest plant source. */
  double distance_meat_;        /*!< Distance to the nearest meat source. */
  double orientation_plant_;   /*!< Orientation relative to the nearest plant
                                 source. */
  double plant_size_;          /*! Size of the closest plant*/
  double meat_size_;          /*! Size of the closest meat*/
  std::shared_ptr<Food> closest_plant_ = nullptr;       /*! Closest plant to show in the UI */
  std::shared_ptr<Food> closest_meat_ = nullptr;      /*! Closest meat to show in the UI */
  double orientation_meat_;   /*!< Orientation relative to the nearest meat
                                source. */
  double vision_radius_; /*!< The radius within which the creature can detect
                            other entities. */
  double vision_angle_;  /*!< The angle of vision for the creature, representing
                            the field of view. */
};

double GetRandomFloat(double min_value, double max_value);
#endif // VISIONSYSTEM_H
