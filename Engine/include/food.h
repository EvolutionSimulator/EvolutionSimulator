#ifndef FOOD_H
#define FOOD_H

#include <entity.h>

#include "environment.h"

class Food : public Entity {
 public:
  Food();
  Food(const double x_coord, const double y_coord);
  Food(const double x_coord, const double y_coord, const double size);
  Food(const double size);

  void Eat();
  void SetNutritionalValue(double value);
  double GetNutritionalValue() const;

  void SetLifespan(int lifespan);
  int GetLifespan() const;

 protected:
  double nutritional_value_; /*!< Nutritional value per size unit of the Food. (depends on food type) */
  int lifespan_; /*!< Lifespan of the Food in simulation time units. */
};

class Plant : public Food {
 public:
  void Grow(double deltaTime);

 protected:
  double age_ = 0.0; /*!< Age of the Plant in simulation time units. */
};

class Meat : public Food {
 public:
  void Rot();
};

#endif  // FOOD_H
