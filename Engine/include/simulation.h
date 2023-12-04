#pragma once

#include "simulationdata.h"
#include <functional>
#include <mutex>

class Simulation
{
public:
    Simulation();
    ~Simulation();
    SimulationData* GetSimulationData();

    void Start();
    void Update(double deltaTime);
    void FixedUpdate(double deltaTime);
    void Stop(); //Gives us the possibility to stop the simulation
    void ProcessData(std::function<void(SimulationData*)> processFunc);

private:
    SimulationData* data_;
    std::mutex data_mutex_;
    bool is_running_; // New boolean flag to control simulation state
};
