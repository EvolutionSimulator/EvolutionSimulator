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
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    auto print_duration = [&start, &end](const char* function_name) {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << function_name << " took " << duration.count() << " ms\n";
        start = std::chrono::high_resolution_clock::now();
    };

    auto data = GetSimulationData();
    print_duration("GetSimulationData");

    auto environment = data->GetEnvironment();
    print_duration("GetEnvironment");

    creature_manager_.UpdateAllCreatures(*data, environment, entity_grid_, deltaTime);
    print_duration("UpdateAllCreatures");

    creature_manager_.ReproduceCreatures(*data, environment);
    print_duration("ReproduceCreatures");

    food_manager_.GenerateMoreFood(*data_, environment, deltaTime);
    print_duration("GenerateMoreFood");

    food_manager_.UpdateAllFood(*data, deltaTime);
    print_duration("UpdateAllFood");

    entity_grid_.UpdateGrid(*data_, environment);
    print_duration("UpdateGrid");

    collision_manager_.CheckCollisions(entity_grid_);
    print_duration("CheckCollisions");

    data_->world_time_ += deltaTime;
    data_->UpdateStatistics();
    print_duration("UpdateTimeAndStatistics");

    std::cout << "World time: " << data_->world_time_ << std::endl;
}

DataAccessor<SimulationData> Simulation::GetSimulationData() {
  return DataAccessor<SimulationData>(*data_, data_sync_);
}

// Function to stop the simulation

void Simulation::Stop() { is_running_ = false; }
