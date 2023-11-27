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

double Entity::GetSize() const
{
    return size_;
}

void Entity::SetSize(double size)
{
    size_ = size;
}

std::pair<double, double> Entity::GetCoordinates() const
{
    return std::make_pair(x_coord_, y_coord_);
}

void Entity::SetCoordinates(const double x, const double y)
{
    x_coord_ = x;
    y_coord_ = y;
}

