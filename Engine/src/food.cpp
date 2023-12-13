#include "food.h"

#include "cstdlib"
#include "environment.h"
Food::Food()
    : Entity(std::rand()%Environment::kMaxFoodSize), nutritional_value_(Environment::kBaseNutritionalValue)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
    : Entity(x_coord, y_coord, std::rand()%Environment::kMaxFoodSize), nutritional_value_(Environment::kBaseNutritionalValue)
{
    //Constructor implementation
}

Food::Food(const double x_coord, const double y_coord, const double size)
    : Entity(x_coord, y_coord, size), nutritional_value_(Environment::kBaseNutritionalValue)
{
    //Constructor implementation
}

Food::Food(const double size)
    : Entity(size)
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

