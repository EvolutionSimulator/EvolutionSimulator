#ifndef FOOD_H
#define FOOD_H

#include <entity.h>

#include "environment.h"

class Food : public Entity {  
 protected:
  Food(const double nutritional_value);
  Food(const double x_coord, const double y_coord, const double nutritional_value);
  Food(const double x_coord, const double y_coord, const double size, const double nutritional_value);
  Food(const double size, const double nutritional_value);

 public:
  enum type { meat, plant };
  void Eat();
  void SetNutritionalValue(double value);
  double GetNutritionalValue() const;

  void SetLifespan(int lifespan);
  int GetLifespan() const;
  type GetType() const;

 protected:
  Food::type type_;
  double nutritional_value_; /*!< Nutritional value per size unit of the Food. (depends on food type) */
  int lifespan_; /*!< Lifespan of the Food in simulation time units. */
};

class Plant : public Food {
 public:
  Plant();
  Plant(const double x_coord, const double y_coord);
  Plant(const double x_coord, const double y_coord, const double size);
  Plant(const double size);
  void Grow(double deltaTime);

 protected:
  double age_ = 0.0; /*!< Age of the Plant in simulation time units. */
};

class Meat : public Food {
 public:
  Meat();
  Meat(const double x_coord, const double y_coord);
  Meat(const double x_coord, const double y_coord, const double size);
  Meat(const double size);
  void Rot(double deltaTime);
};

#endif  // FOOD_H
