#include "food.h"

#include "cstdlib"
#include "environment.h"
/*!
 * @brief Default constructor for Food.
 *
 * @details Initializes Food with a random size and calculates its nutritional
 * value.
 */
Food::Food(const double nutritional_value)
    : Entity(std::rand() % settings::environment::kMaxFoodSize),
      nutritional_value_(size_ * nutritional_value) {
  // Constructor implementation
}

/*!
 * @brief Constructor for Food with specified coordinates.
 *
 * @param x_coord X-coordinate of the Food.
 * @param y_coord Y-coordinate of the Food.
 */
Food::Food(const double x_coord, const double y_coord, const double nutritional_value)
    : Entity(x_coord, y_coord,
             std::rand() % settings::environment::kMaxFoodSize),
      nutritional_value_(size_ * nutritional_value) {}

/*!
 * @brief Constructor for Food with specified coordinates and size.
 *
 * @param x_coord X-coordinate of the Food.
 * @param y_coord Y-coordinate of the Food.
 * @param size Size of the Food.
 */
Food::Food(const double x_coord, const double y_coord, const double size, const double nutritional_value)
    : Entity(x_coord, y_coord, size),
      nutritional_value_(size_ * nutritional_value) {}

/*!
 * @brief Constructor for Food with specified size.
 *
 * @param size Size of the Food.
 */
Food::Food(const double size, const double nutritional_value)
    : Entity(size),
    nutritional_value_(size_ * nutritional_value) {}


/*!
 * @brief Simulates the consumption of the Food, setting its state to Dead.
 */
void Food::Eat() {
  // Call the destructor of the base class Entity
  SetState(Entity::Dead);
}

/*!
 * @brief Sets the nutritional value of the Food.
 *
 * @param value New nutritional value to be set.
 */
void Food::SetNutritionalValue(double value) { nutritional_value_ = value; }

/*!
 * @brief Gets the nutritional value of the Food.
 *
 * @return The current nutritional value.
 */
double Food::GetNutritionalValue() const { return nutritional_value_; }

Food::type Food::GetType() const { return type_;}

Plant::Plant()
    : Food(settings::environment::kPlantNutritionalValue) {
    type_ = plant;
}
Plant::Plant(double x_coord, double y_coord)
    : Food(x_coord, y_coord, settings::environment::kPlantNutritionalValue) {
    type_ = plant;
}
Plant::Plant(double x_coord, double y_coord, double size)
    : Food(x_coord, y_coord, size, settings::environment::kPlantNutritionalValue){
    type_ = plant;
}
Plant::Plant(double size)
    : Food(size, settings::environment::kPlantNutritionalValue){
    type_ = plant;
}

/*!
 * @brief Simulates the growth of the Plant over a period of time.
 *
 * @param deltaTime Time step for the growth simulation.
 *
 * @details The plant's nutritional value increases with photosynthesis and
 * decreases with age.
 */

void Plant::Grow(double deltaTime) {
  double updated_nutritional_value =
      GetNutritionalValue() + settings::environment::kPhotosynthesisFactor;
  if (updated_nutritional_value <=
      settings::environment::kMaxNutritionalValue) {
    SetNutritionalValue(updated_nutritional_value);
  }

  age_ += deltaTime;

  double aging_factor = 0.02;  // Adjust this factor
  updated_nutritional_value = GetNutritionalValue() - aging_factor * age_;

  if (updated_nutritional_value >= 0) {
    SetNutritionalValue(updated_nutritional_value);
  } else {
    SetState(Entity::Dead);
  }
}

Meat::Meat()
    : Food(settings::environment::kPlantNutritionalValue) {
    type_ = meat;
}
Meat::Meat(double x_coord, double y_coord)
    : Food(x_coord, y_coord, settings::environment::kPlantNutritionalValue) {
    type_ = meat;
}
Meat::Meat(double x_coord, double y_coord, double size)
    : Food(x_coord, y_coord, size, settings::environment::kPlantNutritionalValue){
    type_ = meat;
}
Meat::Meat(double size)
    : Food(size, settings::environment::kPlantNutritionalValue){
    type_ = meat;
}

/*!
 * @brief Simulates the rotting process of the Meat.
 *
 * @details The nutritional value of the Meat decreases over time, and it
 * becomes Dead when it reaches zero.
 */
void Meat::Rot() {
  double updated_nutritional_value =
      GetNutritionalValue() - settings::environment::kRotFactor;
  if (updated_nutritional_value < 0) {
    SetState(Entity::Dead);
    return;
  }
  SetNutritionalValue(updated_nutritional_value);
}
