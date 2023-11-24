#pragma once

#include <chrono>

class Simulation;

class Engine
{
public:
    Engine();
    ~Engine();

    void Run();
    void Stop();

    Simulation* GetSimulation();

private:
    const double fixedUpdateInterval = 0.05;
    Simulation* simulation_;
    bool running_ = false;

    typedef std::chrono::system_clock timer;
    timer::time_point engineStartTime_;
    timer::time_point lastUpdateTime_;
    timer::time_point lastFixedUpdateTime_;
};
