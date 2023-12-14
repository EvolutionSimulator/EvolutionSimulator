#ifndef FOOD_H
#define FOOD_H

#include <entity.h>
#include "environment.h"


class Food : public Entity
{
public:

    Food();
    Food(const double x_coord, const double y_coord);
    Food(const double size);

    void Eat();
    void SetNutritionalValue(double value);
    double GetNutritionalValue() const;

    void SetLifespan(int lifespan);
    int GetLifespan() const;


protected:
    double nutritional_value_;
    int lifespan_;
    inline static const double max_nutritional_value_ = 1.0; //to ensure photosynthesis doesn't increase nutrition to infinity
};


class Plant : public Food
{
public:
    void Grow();
private:
    inline const static double max_nutritional_value_ = Environment::kMaxFoodSize * 0.5; //up to 5
    inline const static double default_nutrituonal_value = 0.5;
    inline const static int default_lifespan_ = 30;
    inline const static double photosynthesis_factor_ = 0.1;
    int age_=0;
};

class Meat : public Food
{
public:
    void Rot();

private:
    inline const static double max_nutritional_value_ = Environment::kMaxFoodSize;
    inline const static double default_nutrituonal_value = 10;
    inline const static double rot_factor_ = 1;
};

#endif // FOOD_H
