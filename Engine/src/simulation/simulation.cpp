#include "simulation/simulation.h"

Simulation::Simulation() : data_(new SimulationData()), is_running_(true) {}

Simulation::Simulation(Environment &environment)
    : data_(new SimulationData()), environment_(environment),
      is_running_(true) {
  food_manager_.InitializeFood(*data_, environment_);
  entity_grid_.RefreshGrid(*data_, environment_);
}

Simulation::~Simulation() { delete data_; }

// Called once at the start of the simulation
void Simulation::Start() {
  auto data = GetSimulationData();
  auto environment = GetEnvironment();

  food_manager_.InitializeFood(*data, *environment);
  creature_manager_.InitializeCreatures(*data, *environment);
  entity_grid_.RefreshGrid(*data, *environment);
}

// Called every update cycle
void Simulation::Update(double deltaTime) {
  //  auto data = GetSimulationData();
  //  auto environment = GetEnvironment();
}

// Called at constant intervals (SETTINGS.engine.fixed_update_interval)
void Simulation::FixedUpdate(double deltaTime) {
  auto data = GetSimulationData();
  auto environment = GetEnvironment();

  // std::cout << "Simulation::FixedUpdate: " << data_->world_time_ << std::endl;

  creature_manager_.UpdateAllCreatures(*data, *environment, entity_grid_, deltaTime);
  creature_manager_.ReproduceCreatures(*data, *environment);
  food_manager_.GenerateMoreFood(*data, *environment);
  entity_grid_.RefreshGrid(*data, *environment);
  collision_manager_.CheckCollisions(entity_grid_);

  data_->world_time_ += deltaTime;
}

DataAccessor<SimulationData> Simulation::GetSimulationData() {
  return DataAccessor<SimulationData>(*data_, data_mutex_);
}

DataAccessor<Environment> Simulation::GetEnvironment() {
  return DataAccessor<Environment>(environment_, environment_mutex_);
}

// Function to stop the simulation

void Simulation::Stop() { is_running_ = false; }
