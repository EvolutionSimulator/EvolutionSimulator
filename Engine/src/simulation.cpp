#include "simulation.h"

Simulation::Simulation(Environment& environment)
{
    data_ = new SimulationData(environment);
    is_running_ = true; // Initialize the flag to true
}

Simulation::~Simulation()
{
    delete data_;
}

// Called once at the start of the simulation
void Simulation::Start() {
    std::lock_guard<std::mutex> lock(data_mutex_);
}

// Called every update cycle
void Simulation::Update(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    // Test function (DO NOT USE)

}

// Called at constant intervals
void Simulation::FixedUpdate(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    // Test function (DO NOT USE)
    data_->GenerateMoreFood();
    data_->UpdateAllCreatures(deltaTime);
    data_->CheckCollisions();
    data_->UpdateGrid();

    data_->world_time_ += deltaTime;
}

// Facilitates data processing with external functions in a thread-safe manner
void Simulation::ProcessData(std::function<void (SimulationData*)> processFunc)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    processFunc(data_);
}

SimulationData* Simulation::GetSimulationData() {
    return data_;
}


// Function to stop the simulation

void Simulation::Stop()
{
    is_running_ = false;
}
