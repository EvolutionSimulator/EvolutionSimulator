#include "food.h"
#include "cstdlib"
#include "environment.h"
Food::Food()
    : Entity(std::rand()%Environment::kMaxFoodSize)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
    : Entity(x_coord, y_coord, std::rand()%Environment::kMaxFoodSize)
{
    //Constructor implementation
}

Food::Food(const double size)
    : Entity(size)
{

}

void Food::Eat()
{
    // Call the destructor of the base class Entity
    this->Entity::~Entity();
}


