#include "entity.h"
#include <random>
#include <cmath>

// Entity constructor
Entity::Entity()
    : x_coord_(0.0), y_coord_(0.0), size_(0.0)
{

}

Entity::Entity(const double x_coord, const double y_coord, const double size)
    : x_coord_(x_coord), y_coord_(y_coord), size_(size)
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

double GetRandomFloat(double world_size) {
    // Create a random number generator engine
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a distribution for random floats between 0 and world_size
    std::uniform_real_distribution<double> dis(0.0, world_size);

    // Generate a random float
    return dis(gen);
}

void Entity::RandomInitialization(const double world_width, const double world_height)
{
    x_coord_ = GetRandomFloat(world_width);
    y_coord_ = GetRandomFloat(world_height);
}

double Entity::GetDistance(const Entity& other_entity) const {
    std::pair<double, double> other_coordinates = other_entity.GetCoordinates();

    // Use std::hypot for optimized distance calculation
    return std::hypot(x_coord_ - other_coordinates.first, y_coord_- other_coordinates.second);
}

double Entity::GetOrientation() const
{
    return orientation_;
}

void Entity::SetOrientation(double orientation)
{
    orientation_ = orientation;
}
