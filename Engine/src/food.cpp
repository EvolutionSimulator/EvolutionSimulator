#include "food.h"

#include "cstdlib"
#include "environment.h"
#include "settings.h"

/*!
 * @brief Default constructor for Food.
 *
 * @details Initializes Food with a random size and calculates its nutritional
 * value.
 */
Food::Food(const double nutritional_value)
    : Entity(std::rand() % SETTINGS.environment.max_food_size),
      nutritional_value_(size_ * nutritional_value){
}

/*!
 * @brief Constructor for Food with specified coordinates.
 *
 * @param x_coord X-coordinate of the Food.
 * @param y_coord Y-coordinate of the Food.
 */
Food::Food(const double x_coord, const double y_coord, const double nutritional_value)
    : Entity(x_coord, y_coord,
             std::rand() % SETTINGS.environment.max_food_size),
      nutritional_value_(nutritional_value) {}

/*!
 * @brief Constructor for Food with specified coordinates and size.
 *
 * @param x_coord X-coordinate of the Food.
 * @param y_coord Y-coordinate of the Food.
 * @param size Size of the Food.
 */
Food::Food(const double x_coord, const double y_coord, const double size, const double nutritional_value)
    : Entity(x_coord, y_coord, size),
      nutritional_value_(nutritional_value) {}

/*!
 * @brief Constructor for Food with specified size.
 *
 * @param size Size of the Food.
 */
Food::Food(const double size, const double nutritional_value)
    : Entity(size),
    nutritional_value_(nutritional_value) {}


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

void Food::Update(double deltaTime) {}

Food::type Food::GetType() const { return type_;}

Plant::Plant()
    : Food(SETTINGS.environment.plant_nutritional_value) {
    type_ = plant;
}
Plant::Plant(double x_coord, double y_coord)
    : Food(x_coord, y_coord, SETTINGS.environment.plant_nutritional_value) {
    type_ = plant;
}
Plant::Plant(double x_coord, double y_coord, double size)
    : Food(x_coord, y_coord, size, SETTINGS.environment.plant_nutritional_value){
    type_ = plant;
}
Plant::Plant(double size)
    : Food(size, SETTINGS.environment.plant_nutritional_value){
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

void Plant::Update(double deltaTime) {
  double updated_nutritional_value =
      GetNutritionalValue() + SETTINGS.environment.photosynthesis_factor * deltaTime;
  double aging_factor = 0.002;
  if (updated_nutritional_value >
      SETTINGS.environment.max_nutritional_value * std::exp(-aging_factor * age_)) {
    updated_nutritional_value = SETTINGS.environment.max_nutritional_value * std::exp(-aging_factor * age_);
  }
  age_ += deltaTime;
    // Adjust this factor
  if (updated_nutritional_value >= 0.01) {
    SetNutritionalValue(updated_nutritional_value);
  } else {
    SetState(Entity::Dead);
  }
  SetColor((updated_nutritional_value/SETTINGS.environment.plant_nutritional_value)*0.03);
}

Meat::Meat()
    : Food(SETTINGS.environment.meat_nutritional_value) {
    type_ = meat;
}
Meat::Meat(double x_coord, double y_coord)
    : Food(x_coord, y_coord, SETTINGS.environment.meat_nutritional_value) {
    type_ = meat;
}
Meat::Meat(double x_coord, double y_coord, double size)
    : Food(x_coord, y_coord, size, SETTINGS.environment.meat_nutritional_value){
    type_ = meat;
}
Meat::Meat(double size)
    : Food(size, SETTINGS.environment.meat_nutritional_value){
    type_ = meat;
}

/*!
 * @brief Simulates the rotting process of the Meat.
 *
 * @details The nutritional value of the Meat decreases over time, and it
 * becomes Dead when it reaches zero.
 */
void Meat::Update(double deltaTime) {
  double updated_nutritional_value =
      GetNutritionalValue() - SETTINGS.environment.rot_factor * deltaTime;
  if (updated_nutritional_value < -0.5) { //To see if creatures will
    SetState(Entity::Dead);
    return;
  }

  SetColor((1-updated_nutritional_value/SETTINGS.environment.meat_nutritional_value)/7);
  SetNutritionalValue(updated_nutritional_value);
}
