#ifndef FOOD_H
#define FOOD_H

#include <entity.h>
#include "environment.h"


class Food : public Entity
{
public:

    Food();
    Food(const double x_coord, const double y_coord);
    Food(const double x_coord, const double y_coord, const double size);
    Food(const double size);

    void Eat();
    void SetNutritionalValue(double value);
    double GetNutritionalValue() const;

    void SetLifespan(int lifespan);
    int GetLifespan() const;



protected:
    double nutritional_value_;
    int lifespan_;
};


class Plant : public Food
{
public:
    void Grow();
private:
    int age_=0;
};

class Meat : public Food
{
public:
    void Rot();

private:

};

#endif // FOOD_H
