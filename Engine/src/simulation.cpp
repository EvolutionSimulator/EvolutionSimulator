#include "simulation.h"

Simulation::Simulation()
{
    data_ = new SimulationData();
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
    data_->ModifyAllCreatures(100 * deltaTime, 100 * deltaTime);

}

// Called at constant intervals
void Simulation::FixedUpdate(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    // Test function (DO NOT USE)
    data_->ModifyAllCreatures(100 * deltaTime, 100 * deltaTime);
    data_->CheckFoodCollisions();
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
