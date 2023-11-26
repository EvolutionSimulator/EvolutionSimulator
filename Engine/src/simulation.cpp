#include "simulation.h"

Simulation::Simulation()
{
    data_ = new SimulationData();
}

Simulation::~Simulation()
{
    delete data_;
}

// Called once at the start of the simulation
void Simulation::Start() {

}

// Called every update cycle
void Simulation::Update(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    data_->x += 100 * deltaTime;
}

// Called at constant intervals
void Simulation::FixedUpdate(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    data_->y += 100 * deltaTime;
}

// Facilitates data processing with external functions in a thread-safe manner
void Simulation::ProcessData(std::function<void (SimulationData*)> processFunc)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    processFunc(data_);
}
