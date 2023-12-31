#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "collisions.h"

class Entity {
 public:
  /*!
   * @enum states
   * @brief Enumerates the possible states of an Entity.
   */
  enum states { Dead, Alive };
  Entity();
  Entity(const double x_coord, const double y_coord, const double size);
  Entity(const double size);
  virtual ~Entity();

  void RandomInitialization(const double world_width, const double world_height,
                            const double max_creature_size,
                            const double min_creature_size);

  void RandomInitialization(const double world_width, const double world_height);

  double GetSize() const;
  void SetSize(double size);

  std::pair<double, double> GetCoordinates() const;
  void SetCoordinates(const double x, const double y, const double kMapWidth,
                      const double kMapHeight);

  double GetOrientation() const;
  void SetOrientation(double orientation);

  states GetState() const;
  void SetState(states state);

  virtual void OnCollision(Entity &otherEntity, double const kMapWidth,
                           double const kMapHeight);
  double GetDistance(const Entity &otherEntity, double const kMapWidth,
                     double const kMapHeight) const;
  bool CheckCollisionWithEntity(const double tolerance,
                                const Entity &otherEntity) const {
    return CollisionCircleCircle(tolerance, GetCoordinates(), GetSize(),
                                 otherEntity.GetCoordinates(),
                                 otherEntity.GetSize());
  }
  double GetRelativeOrientation(const Entity &otherEntity) const;

  int GetID() const;

 protected:
  double x_coord_, y_coord_, orientation_, size_;
  states state_;

 private:
  static int next_id_;
  int id_;
};

#endif  // ENTITY_H
