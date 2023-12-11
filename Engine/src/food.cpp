#include "food.h"

#include "cstdlib"
#include "environment.h"
Food::Food()
    : Entity(std::rand()%settings::environment::kMaxFoodSize), nutritional_value_(0.0)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
    : Entity(x_coord, y_coord, std::rand()%settings::environment::kMaxFoodSize), nutritional_value_(settings::environment::kBaseNutritionalValue)
{
    //Constructor implementation
}

Food::Food(const double x_coord, const double y_coord, const double size)
    : Entity(x_coord, y_coord, size), nutritional_value_(settings::environment::kBaseNutritionalValue)
{
    //Constructor implementation
}

void Food::Eat()
{
    SetState(Entity::Dead);
}

void Food::SetNutritionalValue(double value)
{
    nutritional_value_ = value;
}

double Food::GetNutritionalValue() const
{
    return nutritional_value_;
}

