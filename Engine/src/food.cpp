#include "food.h"

Food::Food()
    : Entity(), nutritional_value_(0.0)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
    : Entity(x_coord, y_coord, 0.0), nutritional_value_(0.0)
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
