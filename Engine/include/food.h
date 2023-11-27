#ifndef FOOD_H
#define FOOD_H

#include <entity.h>

class Food : public Entity
{
public:

    Food();

    void Eat();
    void SetNutritionalValue(double value);
    double GetNutritionalValue() const;

private:
    double nutritional_value_;
};


#endif // FOOD_H
