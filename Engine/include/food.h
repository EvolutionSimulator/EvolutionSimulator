#ifndef FOOD_H
#define FOOD_H

#include <entity.h>

class Food : public Entity
{
public:

    Food();
    Food(const double x_coord, const double y_coord);
    Food(const double size);
    void Eat();

private:
};


#endif // FOOD_H
