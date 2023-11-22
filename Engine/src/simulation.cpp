#include "simulation.h"

Simulation::Simulation()
{

}

void Simulation::Update(double deltaTime)
{
    x += 100 * deltaTime;
}

void Simulation::FixedUpdate(double deltaTime)
{
    y += 100 * deltaTime;
}
