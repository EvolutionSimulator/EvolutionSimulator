#include "entity.h"

#include <cassert>
#include <cmath>
#include <random>

#include "environment.h"
#include "geometry_primitives.h"

int Entity::next_id_ = 0;

/*!
 * @brief Default constructor initializing an Entity at the origin with zero
 * size.
 */
Entity::Entity() : x_coord_(0.0), y_coord_(0.0),
    size_(0.0), state_(Alive), orientation_(0),
    id_(next_id_++), color_hue_(0) {}


/*!
 * @brief Parameterized constructor to initialize an Entity with specified
 * coordinates and size.
 *
 * @param x_coord X-coordinate of the entity.
 * @param y_coord Y-coordinate of the entity.
 * @param size Size of the entity.
 */
Entity::Entity(const double x_coord, const double y_coord, const double size)
    : x_coord_(x_coord),
      y_coord_(y_coord),
      size_(size),
      orientation_(0),
      state_(Alive),
      id_(next_id_++),
      color_hue_(0) {
    orientation_ = GetRandomFloat(2*M_PI);
}

/*!
 * @brief Constructor to initialize an Entity with a specified size at the
 * origin.
 * @param size Size of the entity.
 */
Entity::Entity(const double size)
    : x_coord_(0.0),
      y_coord_(0.0),
      size_(size),
      orientation_(0),
      state_(Alive),
      id_(next_id_++),
      color_hue_(0) {}

/*!
 * @brief Destructor sets the state of the entity to Dead.
 */
Entity::~Entity() { }

/*!
 * @brief Retrieves the size of the entity.
 *
 * @return The size of the entity.
 */
double Entity::GetSize() const { return size_; }

/*!
 * @brief Sets the size of the entity.
 *
 * @param size New size of the entity.
 */
void Entity::SetSize(double size) { size_ = size; }

/*!
 * @brief Gets the current coordinates of the entity as a pair.
 *
 * @return A pair representing the X and Y coordinates.
 */
std::pair<double, double> Entity::GetCoordinates() const {
  return std::make_pair(x_coord_, y_coord_);
}

/*!
 * @brief Sets the coordinates of the entity, ensuring they are within the
 * bounds of the map.
 *
 * @details Coordinates are adjusted using the modulo operation to wrap around
 * the map boundaries.
 *
 * @param x New X-coordinate of the entity.
 * @param y New Y-coordinate of the entity.
 * @param kMapWidth Width of the map, used for wrapping coordinates.
 * @param kMapHeight Height of the map, used for wrapping coordinates.
 */
void Entity::SetCoordinates(const double x, const double y,
                            const double kMapWidth, const double kMapHeight) {
  // Use fmod to ensure coordinates are within map bounds
  x_coord_ = fmod(x, kMapWidth);
  y_coord_ = fmod(y, kMapHeight);
  if (x_coord_ < 0.0) x_coord_ += kMapWidth;
  if (y_coord_ < 0.0) y_coord_ += kMapHeight;
}

void Entity::SetCoordinatesNoWrap(const double x, const double y) {
  x_coord_ = x;
  y_coord_ = y;
}
/*!
 * @brief Generates a random floating-point number within a given range.
 *
 * @details This function uses a uniform distribution to ensure an even spread
 * of values.
 *
 * @param world_size The upper limit of the random number range.
 *
 * @return A random floating-point number between 0 and world_size.
 */
double GetRandomFloat(double world_size) {
  // Create a random number generator engine
  std::random_device rd;
  std::mt19937 gen(rd());

  // Define a distribution for random floats between 0 and world_size
  std::uniform_real_distribution<double> dis(0.0, world_size);
  return dis(gen);
}

/*!
 * @brief Randomly initializes the entity's position and size within the world
 * bounds.
 *
 * @param world_width Maximum width of the world for positioning.
 * @param world_height Maximum height of the world for positioning.
 * @param max_creature_size Maximum possible size of the creature.
 * @param min_creature_size Minimum possible size of the creature.
 */
void Entity::RandomInitialization(const double world_width,
                                  const double world_height,
                                  const double max_creature_size,
                                  const double min_creature_size) {
  x_coord_ = GetRandomFloat(world_width);
  y_coord_ = GetRandomFloat(world_height);
  size_ =
      GetRandomFloat(max_creature_size - min_creature_size) + min_creature_size;
  orientation_ = GetRandomFloat(2*M_PI) - M_PI;
}

void Entity::RandomInitialization(const double world_width,
                                  const double world_height) {
  x_coord_ = GetRandomFloat(world_width);
  y_coord_ = GetRandomFloat(world_height);
  orientation_ = GetRandomFloat(2*M_PI);
}

/*!
 * @brief Calculates the distance between this entity and another entity,
 * considering world wrap-around.
 *
 * @param other_entity The other entity to measure distance to.
 * @param kMapWidth Width of the map, used for calculating wrap-around distance.
 * @param kMapHeight Height of the map, used for calculating wrap-around
 * distance.
 *
 * @return The shortest distance between this entity and another, considering
 * map boundaries.
 */
double Entity::GetDistance(const std::shared_ptr<Entity> other_entity, const double kMapWidth,
                           const double kMapHeight) const {
  std::pair<double, double> other_coordinates = other_entity->GetCoordinates();

  // Use std::hypot for optimized distance calculation
  const double x_diff = fabs(x_coord_ - other_coordinates.first);
  const double y_diff = fabs(y_coord_ - other_coordinates.second);
  return std::hypot(fmin(x_diff, kMapWidth - x_diff),
                    fmin(y_diff, kMapHeight - y_diff));
}

/*!
 * @brief Calculates the relative orientation from this entity to another
 * entity.
 *
 * @param other_entity The other entity to calculate orientation towards.
 *
 * @return The relative orientation angle in radians between [-pi,pi].
 */
double Entity::GetRelativeOrientation(const std::shared_ptr<Entity> other_entity) const {
  // assumes orientation = 0 is the x axis
  return (OrientedAngle(Point(GetCoordinates()),
                        Point(other_entity->GetCoordinates())) -
          OrientedAngle(orientation_))
      .GetAngle();
}

/*!
 * @brief Retrieves the current orientation of the entity.
 *
 * @return The current orientation angle of the entity in radians.
 */
double Entity::GetOrientation() const { return orientation_; }

/*!
 * @brief Sets the orientation of the entity.
 *
 * @param orientation New orientation angle in radians.
 */
void Entity::SetOrientation(double orientation) { orientation_ = orientation; }

/*!
 * @brief Retrieves the current state of the entity.
 *
 * @return The current state of the entity (Alive or Dead).
 */
Entity::states Entity::GetState() const { return state_; }

/*!
 * @brief Sets the state of the entity.
 *
 * @param state New state of the entity (Alive or Dead).
 */
void Entity::SetState(Entity::states state) { state_ = state; }

/*!
 * @brief Handles the collision between this entity and another entity.
 *
 * @details This method moves the smaller entity to resolve overlaps based on
 * their sizes.
 *
 * @param other_entity The other entity involved in the collision.
 * @param kMapWidth Width of the map, used for position adjustments.
 * @param kMapHeight Height of the map, used for position adjustments.
 */
void Entity::OnCollision(std::shared_ptr<Entity> other_entity, double const kMapWidth,
                         double const kMapHeight) { }

int Entity::GetID() const {
  return id_;
}

float Entity::GetColor() const {
  return color_hue_;
}

void Entity::SetColor(float value){
  color_hue_ = fmod(value, 360);
}
