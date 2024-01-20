#include "simulation.h"
#include <chrono>

Simulation::Simulation(Environment& environment):
    food_manager_() {
  data_ = new SimulationData(environment);
  is_running_ = true;  // Initialize the flag to true
}

Simulation::~Simulation() { delete data_; }

// Called once at the start of the simulation
void Simulation::Start() {
  auto data = GetSimulationData();
  auto environment = data->GetEnvironment();

  food_manager_.InitializeFood(*data, environment);
  creature_manager_.InitializeCreatures(*data, environment);
}

// Called every update cycle
void Simulation::Update(double deltaTime) {
  auto data = GetSimulationData();
  // Test function (DO NOT USE)
}

// Called at constant intervals
void Simulation::FixedUpdate(double deltaTime) {
  auto data = GetSimulationData();
  auto environment = data->GetEnvironment();

  creature_manager_.UpdateAllCreatures(*data, environment, entity_grid_, deltaTime);
  creature_manager_.ReproduceCreatures(*data, environment);
  food_manager_.GenerateMoreFood(*data_, environment, deltaTime);
  entity_grid_.UpdateGrid(*data_, environment);
  collision_manager_.CheckCollisions(entity_grid_);
  data_->world_time_ += deltaTime;
  data_ -> UpdateStatistics();

  std::cout << "World time: " << data_->world_time_ << std::endl;
}

DataAccessor<SimulationData> Simulation::GetSimulationData() {
  return DataAccessor<SimulationData>(*data_, data_sync_);
}

// Function to stop the simulation

void Simulation::Stop() { is_running_ = false; }
