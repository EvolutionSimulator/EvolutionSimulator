#pragma once

#include "simulation.h"
#include "environment.h"
#include <chrono>

class Simulation;

class Engine
{
public:
    Engine();
    ~Engine();

    void Run();
    void Stop();
    void UpdateEnvironment();

    Simulation* GetSimulation();
    Environment& GetEnvironment();

private:
    Environment environment_;
    const double fixedUpdateInterval = 0.05; // how often FixedUpdate is called
    Simulation* simulation_;
    bool running_ = false;

    typedef std::chrono::system_clock timer;
    timer::time_point engineStartTime_;
    timer::time_point lastUpdateTime_;
    timer::time_point lastFixedUpdateTime_;
};
