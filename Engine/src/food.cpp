#include "food.h"

#include "cstdlib"
#include "environment.h"
Food::Food()
    : Entity(std::rand()%settings::environment::kMaxFoodSize), nutritional_value_(size_*settings::environment::kPlantNutritionalValue)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
    : Entity(x_coord, y_coord, std::rand()%settings::environment::kMaxFoodSize), nutritional_value_(size_*settings::environment::kPlantNutritionalValue)
{
    //Constructor implementation
}

Food::Food(const double x_coord, const double y_coord, const double size)
    : Entity(x_coord, y_coord, size), nutritional_value_(size_*settings::environment::kPlantNutritionalValue)
{
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


void Plant::Grow()
{
    double updated_nutritional_value = GetNutritionalValue() + settings::environment::kPhotosynthesisFactor;
    if (updated_nutritional_value <= settings::environment::kMaxNutritionalValue)
    {
        SetNutritionalValue(updated_nutritional_value);
    }

    age_++;

    double aging_factor = 0.02; // Adjust this factor
    updated_nutritional_value = GetNutritionalValue() - aging_factor * age_;

    if (updated_nutritional_value >= 0)
    {
        SetNutritionalValue(updated_nutritional_value);
    }
    else
    {
        SetState(Entity::Dead);
    }
}


void Meat::Rot()
{
    double updated_nutritional_value = GetNutritionalValue() - settings::environment::kRotFactor;
    if (updated_nutritional_value < 0)
    {
        SetState(Entity::Dead);
        return;
    }
    SetNutritionalValue(updated_nutritional_value);
}
