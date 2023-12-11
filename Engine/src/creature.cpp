#include "creature.h"
#include <algorithm>
#include <cassert>

Creature::Creature(neat::Genome genome)
    : MovableEntity(), energy_(0.0), brain_(neat::NeuralNetwork(genome)),
      genome_(genome) {}

double Creature::GetEnergy() const { return energy_; }

void Creature::SetEnergy(double energy) { energy_ = energy; }

void Creature::UpdateEnergy() {
  SetEnergy(GetEnergy() -
            (GetVelocityForward() + GetRotationalVelocity()) * GetSize());

  if (GetEnergy() <= 0) {
    Dies();
  }
}

void Creature::Dies() { SetState(Dead); }

void Creature::Eats(double nutritional_value) {
  SetEnergy(GetEnergy() + nutritional_value);
}

bool Creature::Fit() {
  if (energy_ > cfg::reproduction_threshold) {
    return true;
  }
  return false;
}

void Creature::Update(double deltaTime, double const kMapWidth,
                      double const kMapHeight) {
  this->Move(deltaTime, kMapWidth, kMapHeight);
  this->Rotate(deltaTime);
}

neat::Genome Creature::GetGenome() { return genome_; }
void Creature::OnCollision(Food &food) {
  if (food.GetState() == Entity::Alive) {
    Eats(food.GetNutritionalValue());
    food.Eat();
  }
}

void Creature::OnCollision(Creature &creature) {}

void Creature::SetGrowthFactor(double growth_factor) {
  growth_factor_ = growth_factor;
}

double Creature::GetGrowthFactor() { return growth_factor_; }

void Creature::SetMaxSize(double max_size) { max_size_ = max_size; }

double Creature::GetMaxSize() { return max_size_; }

void Creature::Grow(double energy) {
  double size = GetSize() + energy * GetGrowthFactor();
  (size > GetMaxSize()) ? SetSize(GetMaxSize()) : SetSize(size);
  SetEnergy(GetEnergy() - energy);
}

Food *Creature::GetClosestFood(
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity *>>>
        &grid,
    double GridCellSize) const {
  std::pair<double, double> coordinates_creature = GetCoordinates();
  int i_creature = (int)coordinates_creature.first / (int)GridCellSize;
  int j_creature = (int)coordinates_creature.second /
                   (int)GridCellSize; // position of the creature on the grid
  std::vector<Food *> closest_food_entities = get_food_at_distance(
      grid, i_creature, j_creature,
      0); // here we place the candidates for the closest food
  int grid_distance = 1;
  int boundary = std::max(grid.size(), grid[0].size());

  while (closest_food_entities.size() == 0 && grid_distance < boundary) {
    // std::cout << "checking at distance " << grid_distance << std::endl;
    closest_food_entities =
        get_food_at_distance(grid, i_creature, j_creature, grid_distance);
    grid_distance++;
  }
  assert(!closest_food_entities.empty());

  Food *closest_food = closest_food_entities.front();
  double smallest_distance = GetDistance(*closest_food);
  for (Food *&food : closest_food_entities) {
    double distance = GetDistance(*food);
    if (distance < smallest_distance) {
      closest_food = food;
      smallest_distance = distance;
    }
  }
  return closest_food;
}

std::vector<Food *> get_food_at_distance(
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity *>>>
        &grid,
    int i_creature, int j_creature, int grid_distance) {
  std::vector<Food *> food;
  int grid_width = grid.size();
  int grid_height = grid[0].size();

  if (i_creature + grid_distance < grid_width) {
    for (int j = std::max(0, j_creature - grid_distance);
         j < std::min(grid_height, j_creature + grid_distance + 1); j++) {
      // std:: cout << "checking " << i_creature + grid_distance << ", " << j <<
      // std::endl;
      for (std::vector<Entity *>::iterator it =
               grid[i_creature + grid_distance][j].begin();
           it != grid[i_creature + grid_distance][j].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  if (i_creature - grid_distance >= 0) {
    for (int j = std::max(0, j_creature - grid_distance);
         j < std::min(grid_height, j_creature + grid_distance + 1); j++) {
      for (std::vector<Entity *>::iterator it =
               grid[i_creature - grid_distance][j].begin();
           it != grid[i_creature - grid_distance][j].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  if (j_creature + grid_distance < grid_height) {
    for (int i = std::max(0, i_creature - grid_distance + 1);
         i < std::min(grid_width, i_creature + grid_distance); i++) {
      for (std::vector<Entity *>::iterator it =
               grid[i][j_creature + grid_distance].begin();
           it != grid[i][j_creature + grid_distance].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  if (j_creature - grid_distance >= 0) {
    for (int i = std::max(0, i_creature - grid_distance + 1);
         i < std::min(grid_width, i_creature + grid_distance); i++) {
      for (std::vector<Entity *>::iterator it =
               grid[i][j_creature - grid_distance].begin();
           it != grid[i][j_creature - grid_distance].end(); it++) {
        Entity *entity_ptr = *it;
        Food *food_ptr = dynamic_cast<Food *>(entity_ptr);
        if (food_ptr != nullptr) {
          food.push_back(food_ptr);
        }
      }
    }
  }
  return food;
}
