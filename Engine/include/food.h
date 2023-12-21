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
  Plant() : Food(settings::environment::kPlantNutritionalValue){};
  Plant(const double x_coord, const double y_coord) : Food(x_coord, y_coord, settings::environment::kPlantNutritionalValue){};
  Plant(const double x_coord, const double y_coord, const double size) : Food(x_coord, y_coord, size, settings::environment::kPlantNutritionalValue){};
  Plant(const double size) : Food(size, settings::environment::kPlantNutritionalValue){};
  void Grow(double deltaTime);

 protected:
  double age_ = 0.0; /*!< Age of the Plant in simulation time units. */
};

class Meat : public Food {
 public:
  Meat() : Food(settings::environment::kMeatNutritionalValue){};
  Meat(const double x_coord, const double y_coord) : Food(x_coord, y_coord, settings::environment::kMeatNutritionalValue){};
  Meat(const double x_coord, const double y_coord, const double size) : Food(x_coord, y_coord, size, settings::environment::kMeatNutritionalValue){};
  Meat(const double size) : Food(size, settings::environment::kMeatNutritionalValue){};
  void Rot();
};

#endif  // FOOD_H
