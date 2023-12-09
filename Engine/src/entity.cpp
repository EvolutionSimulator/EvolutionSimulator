#include "entity.h"
#include <random>
#include <cmath>

// Entity constructor
Entity::Entity()
    : x_coord_(0.0), y_coord_(0.0), size_(0.0), state_(Alive)
{

}

Entity::Entity(const double x_coord, const double y_coord, const double size)
    : x_coord_(x_coord), y_coord_(y_coord), size_(size), state_(Alive)
{

}

Entity::Entity(const double size)
    : x_coord_(0.0), y_coord_(0.0), size_(size), state_(Alive)
{

}

// Entity destructor
Entity::~Entity()
{
    state_ = Dead;
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

void Entity::SetCoordinates(const double x, const double y, const double kMapWidth, const double kMapHeight)
{
    // Use fmod to ensure coordinates are within map bounds
    x_coord_ = fmod(x, kMapWidth);
    y_coord_ = fmod(y, kMapHeight);
    if (x_coord_ < 0.0)
        x_coord_ += kMapWidth;
    if (y_coord_ < 0.0)
        y_coord_ += kMapHeight;
}

double GetRandomFloat(double world_size) {
    // Create a random number generator engine
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a distribution for random floats between 0 and world_size
    std::uniform_real_distribution<double> dis(0.0, world_size);
    return dis(gen);
}

void Entity::RandomInitialization(const double world_width, const double world_height, const double max_creature_size)
{
    x_coord_ = GetRandomFloat(world_width);
    y_coord_ = GetRandomFloat(world_height);
    size_ = GetRandomFloat(max_creature_size);
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

Entity::states Entity::GetState() const{
    return state_;
}

void Entity::SetState(Entity::states state){
    state_ = state;
}
