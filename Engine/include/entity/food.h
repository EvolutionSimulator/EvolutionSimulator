#ifndef FOOD_H
#define FOOD_H

#include "entity/movable_entity.h"

#include "simulation/environment.h"

class Food : public MovableEntity {
 protected:
  Food(const double nutritional_value);
  Food(const double x_coord, const double y_coord,
       const double nutritional_value);
  Food(const double x_coord, const double y_coord, const double size,
       const double nutritional_value);

 public:
  enum type { meat, plant, egg };
  void Eat();
  void SetNutritionalValue(double value);
  double GetNutritionalValue() const;

  void SetLifespan(int lifespan);
  int GetLifespan() const;
  type GetType() const;

  virtual void Update(double deltaTime);

 protected:
  Food::type type_;
  double nutritional_value_; /*!< Nutritional value per size unit of the Food.
                                (depends on food type) */
};

class Plant : public Food {
 public:
  Plant();
  Plant(const double x_coord, const double y_coord);
  Plant(const double x_coord, const double y_coord, const double size);
  virtual void Update(double deltaTime) override;

 protected:
  double age_ = 0.0; /*!< Age of the Plant in simulation time units. */
};

class Meat : public Food {
 public:
  Meat();
  Meat(const double x_coord, const double y_coord);
  Meat(const double x_coord, const double y_coord, const double size);
  virtual void Update(double deltaTime) override;

};

#endif  // FOOD_H
