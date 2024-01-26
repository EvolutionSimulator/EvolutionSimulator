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
  double GetEntityCompatibility() const;

  std::shared_ptr<Entity> GetFoodID() const;

  bool IsInRightDirection(std::shared_ptr<Entity> entity, double map_width, double map_heigth);
  bool IsInVisionCone(std::shared_ptr<Entity> entity, double map_width, double map_heigth) const;

  std::vector<std::shared_ptr<Entity>> GetClosestEntityInSight(std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> &grid,
                                              double grid_cell_size, double map_width, double map_heigth) const;

protected:
  double distance_entity_;       /*!< Distance to the nearest entity */
  double orientation_entity_;   /*!< Orientation relative to the nearest entity */
  double entity_compatibility_; /*! Compatibility with closest entity*/
  double entity_size_;          /*! Size of the closest entity*/
  double entity_color_;  /*! Color of the closest entity*/
  std::shared_ptr<Entity> closest_entity_;       /*! Closest entity to show in the UI */

  double vision_radius_; /*!< The radius within which the creature can detect
                            other entities. */
  double vision_angle_;  /*!< The angle of vision for the creature, representing
                            the field of view. */

  int number_entities_to_return_; /*!< Number of entities in sight to return when processing vision */
};

double GetRandomFloat(double min_value, double max_value);
#endif // VISIONSYSTEM_H
