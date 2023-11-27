#pragma once

#include "simulationdata.h"
#include <functional>
#include <mutex>

class Simulation
{
public:
    Simulation();
    ~Simulation();

    void Start();
    void Update(double deltaTime);
    void FixedUpdate(double deltaTime);

    void ProcessData(std::function<void(SimulationData*)> processFunc);

private:
    SimulationData* data_;
    std::mutex data_mutex_;
};
