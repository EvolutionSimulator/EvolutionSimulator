#define ENABLE_TIMING
#include "simulation.h"
#include <chrono>


Simulation::Simulation(Environment& environment) : food_manager_() {
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
#ifdef ENABLE_TIMING //To togle the timing just comment/uncomment the #define line
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    auto print_duration = [&start, &end](const char* function_name) {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << function_name << " took " << duration.count() << " ms\n";
        start = std::chrono::high_resolution_clock::now();
    };
    #endif

    auto data = GetSimulationData();
#ifdef ENABLE_TIMING
    print_duration("GetSimulationData");
    #endif

    auto environment = data->GetEnvironment();
#ifdef ENABLE_TIMING
    print_duration("GetEnvironment");
    #endif

    creature_manager_.UpdateAllCreatures(*data, environment, entity_grid_, deltaTime);
#ifdef ENABLE_TIMING
    print_duration("UpdateAllCreatures");
    #endif

    creature_manager_.ReproduceCreatures(*data, environment);
#ifdef ENABLE_TIMING
    print_duration("ReproduceCreatures");
    #endif

    creature_manager_.HatchEggs(*data, environment);
#ifdef ENABLE_TIMING
    print_duration("HatchEggs");
#endif

    food_manager_.GenerateMoreFood(*data_, environment, deltaTime);
#ifdef ENABLE_TIMING
    print_duration("GenerateMoreFood");
    #endif

    food_manager_.UpdateAllFood(*data, deltaTime);
#ifdef ENABLE_TIMING
    print_duration("UpdateAllFood");
    #endif

    entity_grid_.UpdateGrid(*data_, environment, deltaTime);
#ifdef ENABLE_TIMING
    print_duration("UpdateGrid");
    #endif

    collision_manager_.CheckCollisions(entity_grid_);
#ifdef ENABLE_TIMING
    print_duration("CheckCollisions");
    #endif

    data_->world_time_ += deltaTime;
    data_->UpdateStatistics();
#ifdef ENABLE_TIMING
    print_duration("UpdateTimeAndStatistics");
    #endif

    std::cout << "World time: " << data_->world_time_ << std::endl;
}

DataAccessor<SimulationData> Simulation::GetSimulationData() {
  return DataAccessor<SimulationData>(*data_, data_sync_);
}

// Function to stop the simulation

void Simulation::Stop() { is_running_ = false; }
