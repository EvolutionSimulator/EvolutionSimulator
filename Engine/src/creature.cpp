#include "creature.h"
#include <algorithm>
#include <cassert>

Creature::Creature(neat::Genome genome)
    : MovableEntity(), health_(100), energy_(100), brain_(neat::NeuralNetwork(genome)), genome_(genome), neuron_data_(settings::environment::kInputNeurons,0) {
}

int Creature::GetGeneration() const {return generation_;}

void Creature::SetGeneration(int generation){
    generation_ = generation;
}

double Creature::GetEnergy() const { return energy_; }

void Creature::HealthToEnergy() {
    if (GetEnergy() < 0) {
        SetHealth(GetHealth() + GetEnergy()-5);
        SetEnergy(5);
    }
    SetEnergy(GetEnergy() + 5);
    SetHealth(GetHealth() - 5);
}

void Creature::EnergyToHealth() {
    SetEnergy(GetEnergy() - 5);
    SetHealth(GetHealth() + 5);
}

double Creature::GetHealth() const {
    return health_;
}

void Creature::SetHealth(double health) {
    if (health > 100) {
        health_ = 100;
    } else {
        health_ = health;
    }
}

void Creature::Dies() { SetState(Dead); }

void Creature::SetEnergy(double energy) {
    if (energy > max_energy_) {
        energy_ = 100;
    } else {
        energy_ = energy;
    }
}

void Creature::UpdateEnergy(const double energyToHealth, const double healthToEnergy, double deltaTime){
    SetEnergy( GetEnergy() - (GetVelocityForward() + GetRotationalVelocity() + 50) * GetSize() * deltaTime/100);

    if (GetEnergy() <= healthToEnergy){
        HealthToEnergy();
    } else if (GetEnergy() >= energyToHealth){
        EnergyToHealth();
    }

    if (GetHealth() <= 0){
        Dies();
    }
}
bool Creature::Fit() {
    if (energy_ > settings::environment::kReproductionThreshold*max_energy_) {
    return true;
  }
  return false;
}

void Creature::Eats(double nutritional_value){
  SetEnergy(GetEnergy() + nutritional_value);
  if (GetEnergy() > max_energy_) {
    EnergyToHealth();
  }
}

void Creature::Update(double deltaTime, double const kMapWidth,
                      double const kMapHeight,
                      std::vector<std::vector<std::vector<Entity*> > > &grid,
                      double GridCellSize) {
  this->UpdateEnergy(70, 5, deltaTime);
  this->Move(deltaTime, kMapWidth, kMapHeight);
  this->Rotate(deltaTime);
  this->Think(grid, GridCellSize);
}

neat::Genome Creature::GetGenome() { return genome_; }

void Creature::OnCollision(Entity &other_entity, double const kMapWidth,
                           double const kMapHeight)
{
    if (Food* food = dynamic_cast<Food*>(&other_entity))
    {
        if (food->GetState() == Entity::Alive)
        {
            Eats(food->GetNutritionalValue() * food->GetSize());
            food->Eat();
        }
    }
    else
    {
        Entity::OnCollision(other_entity, kMapWidth, kMapHeight);
    }
}

void Creature::SetGrowthFactor(double growth_factor) {
  growth_factor_ = growth_factor;
}

double Creature::GetGrowthFactor() { return growth_factor_; }

void Creature::Think(std::vector<std::vector<std::vector<Entity*> > > &grid, double GridCellSize)
{
    //Not pretty but we'll figure out a better way in the future
    ProcessVisionFood(grid, GridCellSize);
    neuron_data_.at(0) = energy_;
    neuron_data_.at(1) = velocity_forward_;
    neuron_data_.at(2) = rotational_velocity_;
    neuron_data_.at(3) = orientation_food_;
    neuron_data_.at(4) = distance_food_;
    std::vector<double> output = brain_.Activate(neuron_data_);
    if(output.at(0) > 50){
        velocity_forward_=50;
    } else {
        velocity_forward_ = output.at(0) > 0 ? output.at(0) : 0;
    }

    rotational_velocity_ = output.at(1);
}

void Creature::ProcessVisionFood(std::vector<std::vector<std::vector<Entity*> > > &grid, double GridCellSize)
{
    Food* food = this->GetClosestFood(grid, GridCellSize);
    distance_food_ = this->GetDistance(*food);
    orientation_food_ = this->GetRelativeOrientation(*food);
}
double Creature::GetMaxSize() { return max_size_; }
void Creature::SetMaxSize(double max_size) { max_size_ = max_size; }

void Creature::Grow(double energy) {
  double size = GetSize() + energy * GetGrowthFactor();
  (size > GetMaxSize()) ? SetSize(GetMaxSize()) : SetSize(size);
  SetEnergy(GetEnergy() - energy);
}


Food *Creature::GetClosestFood(
    std::vector<std::vector<std::vector<Entity*> > > &grid,
    double GridCellSize) const {
  if (grid.empty()) return nullptr;
  std::pair<double, double> coordinates_creature = GetCoordinates();
  int i_creature = (int)coordinates_creature.first / (int)GridCellSize;
  int j_creature = (int)coordinates_creature.second /
                   (int)GridCellSize; // position of the creature on the grid
  std::vector<Food *> closest_food_entities = get_food_at_distance(
      grid, i_creature, j_creature, 0); // here we place the candidates for the closest food
  int grid_distance = 1;
  int boundary = std::max(grid.size(), grid[0].size());

  while (closest_food_entities.size() == 0 && grid_distance <= boundary) {
    // std::cout << "checking at distance " << grid_distance << std::endl;
    closest_food_entities =
        get_food_at_distance(grid, i_creature, j_creature, grid_distance);
    grid_distance++;
  }
  //assert(!closest_food_entities.empty());
  if (closest_food_entities.empty()) return nullptr;

  Food *closest_food = closest_food_entities.front();
  double smallest_distance = GetDistance(*closest_food);
  for (Food *&food : closest_food_entities) {
    double distance = GetDistance(*food);
    if (distance < smallest_distance) {
      closest_food = food;
      smallest_distance = distance;
    }
    assert( !closest_food_entities.empty());
    return closest_food;
    }
  return nullptr;
}

std::vector<Food *> get_food_at_distance(
    std::vector<std::vector<std::vector<Entity*> > > &grid,
    int i_creature, int j_creature, int grid_distance) {
  std::vector<Food *> food;
  if (grid.empty()) return food;
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
