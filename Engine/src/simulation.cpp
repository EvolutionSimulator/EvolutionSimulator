#include "simulation.h"
#include <chrono>

Simulation::Simulation(myEnvironment::Environment& environment) {
  data_ = new SimulationData(environment);
  is_running_ = true;  // Initialize the flag to true
}

Simulation::~Simulation() { delete data_; }

// Called once at the start of the simulation
void Simulation::Start() { auto data = GetSimulationData(); }

// Called every update cycle
void Simulation::Update(double deltaTime) {
  auto data = GetSimulationData();
  // Test function (DO NOT USE)
}

// Called at constant intervals
void Simulation::FixedUpdate(double deltaTime) {
  auto data = GetSimulationData();
  // Test function (DO NOT USE)
  data_->UpdateAllCreatures(deltaTime);
  data_->ReproduceCreatures();
  data_->GenerateMoreFood(deltaTime);
  data_->UpdateGrid();
  data_->CheckCollisions();
  data_->world_time_ += deltaTime;
}

DataAccessor<SimulationData> Simulation::GetSimulationData() {
  return DataAccessor<SimulationData>(*data_, data_sync_);
}

// Function to stop the simulation

void Simulation::Stop() { is_running_ = false; }
