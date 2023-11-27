#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

class Entity
{
public:
    Entity();
    virtual ~Entity();

    double getSize() const;
    void setSize(double size);

    std::pair<double, double> getCoordinates() const;
    void setCoordinates(const std::pair<double, double>& coordinates);
    void setCoordinates(const double x, const double y);

private:
    double x_coord_, y_coord_, size_;
};


class Food: public Entity
{
public:

    Food();

    void eat();
    void setNutritionalValue(double value);
    double getNutritionalValue() const;

private:
    double nutritional_value_;
};


#endif // ENTITY_H
