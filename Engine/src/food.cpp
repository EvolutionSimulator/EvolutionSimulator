#include "food.h"

#include "cstdlib"
#include "environment.h"
Food::Food()
    : Entity(std::rand()%Environment::kMaxFoodSize), nutritional_value_(0.0)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
    : Entity(x_coord, y_coord, std::rand()%Environment::kMaxFoodSize), nutritional_value_(0.0)
{
    //Constructor implementation
}

Food::Food(const double size)
    : Entity(size)
{
    SetNutritionalValue(size/Environment::kMaxFoodSize * max_nutritional_value_);
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


// Simulate photosynthesis and update the nutritional value
void Plant::Photosynthesize()
{
    double updated_nutritional_value = GetNutritionalValue() + photosynthesis_factor_;

    if (updated_nutritional_value<=Food::max_nutritional_value_){
        SetNutritionalValue(updated_nutritional_value);
    }

}

void Meat::Rot()
{
    double updated_nutritional_value = GetNutritionalValue() - rot_factor_;

    if (updated_nutritional_value<-3){
        return; //kill the food
    }
    SetNutritionalValue(updated_nutritional_value);
}
