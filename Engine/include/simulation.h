#pragma once

#include "engine.h"

class Simulation
{
    friend void Engine::UpdateSimulation(Simulation*, double deltaTime);
    friend void Engine::FixedUpdateSimulation(Simulation*, double deltaTime);

public:
    Simulation();

    double x = 0;
    double y = 0;

protected:
    void Update(double deltaTime);
    void FixedUpdate(double deltaTime);

};
