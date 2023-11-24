#include "simulation.h"

Simulation::Simulation()
{
    data_ = new SimulationData();
}

Simulation::~Simulation()
{
    delete data_;
}

void Simulation::Update(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    data_->x += 100 * deltaTime;
}

void Simulation::FixedUpdate(double deltaTime)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    data_->y += 100 * deltaTime;
}

void Simulation::ProcessData(std::function<void (SimulationData*)> processFunc)
{
    std::lock_guard<std::mutex> lock(data_mutex_);
    processFunc(data_);
}
