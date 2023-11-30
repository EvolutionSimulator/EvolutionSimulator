#include "food.h"

Food::Food()
    : Entity(), nutritional_value_(0.0)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord,  const double size)
    : Entity(x_coord, y_coord, size), nutritional_value_(0.0)
{
    // Constructor implementation
}

void Food::Eat()
{
    // Call the destructor of the base class Entity
    this->Entity::~Entity();
}

void Food::SetNutritionalValue(double value)
{
    nutritional_value_ = value;
}

double Food::GetNutritionalValue() const
{
    return nutritional_value_;
}
