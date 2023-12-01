#include "food.h"
<<<<<<< HEAD
#include "cstdlib"
#include "environment.h"
Food::Food()
    : Entity(std::rand()%Environment::kMaxFoodSize)
=======
#include <stdlib.h>
#include "environment.h"
Food::Food()
    : Entity()
>>>>>>> 0e157bd (Nutritional value to size)
{
    // Constructor implementation
}

Food::Food(const double x_coord, const double y_coord)
<<<<<<< HEAD
    : Entity(x_coord, y_coord, std::rand()%Environment::kMaxFoodSize)
{
    //Constructor implementation
}

Food::Food(const double size)
    : Entity(size)
{

=======
    : Entity(x_coord, y_coord, rand()%Environment::kMaxFoodSize)
{
    //Constructor implementation
>>>>>>> 0e157bd (Nutritional value to size)
}

void Food::Eat()
{
    // Call the destructor of the base class Entity
    this->Entity::~Entity();
}


