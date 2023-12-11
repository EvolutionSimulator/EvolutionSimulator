#ifndef ENTITY_H
#define ENTITY_H

#include "collisions.h"
#include <vector>

class Entity {
public:
  enum states { Dead = 0, Alive = 1 };
  Entity();
  Entity(const double x_coord, const double y_coord, const double size);
  Entity(const double size);
  virtual ~Entity();

  void RandomInitialization(const double world_width, const double world_height,
                            const double max_creature_size);

  double GetSize() const;
  void SetSize(double size);

  std::pair<double, double> GetCoordinates() const;
  void SetCoordinates(const double x, const double y, const double kMapWidth,
                      const double kMapHeight);

  double GetOrientation() const;
  void SetOrientation(double orientation);

  states GetState() const;
  void SetState(states state);

  double GetDistance(const Entity &otherEntity) const;
  bool CheckCollisionWithEntity(const double tolerance,
                                const Entity &otherEntity) const {
    return CollisionCircleCircle(tolerance, GetCoordinates(), GetSize(),
                                 otherEntity.GetCoordinates(),
                                 otherEntity.GetSize());
  }

private:
  double x_coord_, y_coord_, orientation_, size_;
  states state_;
};

#endif // ENTITY_H
