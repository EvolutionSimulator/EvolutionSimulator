#include "entity.h"

// Entity constructor
Entity::Entity()
    : x_coord_(0.0), y_coord_(0.0), size_(0.0)
{

}

// Entity destructor
Entity::~Entity()
{
    // Destructor implementation
}

double Entity::getSize() const
{
    return size_;
}

void Entity::setSize(double size)
{
    size_ = size;
}

std::pair<double, double> Entity::getCoordinates() const
{
    return std::make_pair(x_coord_, y_coord_);
}

void Entity::setCoordinates(const std::pair<double, double>& coordinates)
{
    x_coord_ = coordinates.first;
    y_coord_ = coordinates.second;
}

void Entity::setCoordinates(const double x, const double y)
{
    x_coord_ = x;
    y_coord_ = y;
}

Food::Food()
    : Entity(), nutritional_value_(0.0)
{
    // Constructor implementation
}

void Food::eat()
{
    // Call the destructor of the base class Entity
    this->Entity::~Entity();
}

void Food::setNutritionalValue(double value)
{
    nutritional_value_ = value;
}

double Food::getNutritionalValue() const
{
    return nutritional_value_;
}
