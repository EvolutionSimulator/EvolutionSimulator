#include "engine.h"
#include "simulation.h"
#include <cmath>

Engine::Engine()
{
    simulation_ = new Simulation();
}

Engine::~Engine()
{
    delete simulation_;
}

void Engine::Run()
{
    if(!running_) {
        running_ = true;

        engineStartTime_ = timer::now();
        lastUpdateTime_ = engineStartTime_;
        lastFixedUpdateTime_ = engineStartTime_;

        while (running_) {
            timer::time_point current_time = timer::now();
            double fixed_update_delta =
                std::chrono::duration<double>(current_time - lastFixedUpdateTime_).count();

            double update_delta =
                std::chrono::duration<double>(current_time - lastUpdateTime_).count();

            int fixed_update_steps = std::floor(fixed_update_delta / fixedUpdateInterval);
            for (int i = 0; i < fixed_update_steps; i++) {
                FixedUpdateSimulation(simulation_, fixedUpdateInterval);
            }

            UpdateSimulation(simulation_, update_delta);

            lastUpdateTime_ = current_time;
            lastFixedUpdateTime_ +=
                std::chrono::duration_cast<timer::time_point::duration>(
                    std::chrono::duration<double>(fixed_update_steps * fixedUpdateInterval)
                );
        }
    }
}

void Engine::Stop()
{
    running_ = false;
}

void Engine::UpdateSimulation(Simulation* simulation, double deltaTime) {
    simulation->Update(deltaTime);
}

void Engine::FixedUpdateSimulation(Simulation* simulation, double deltaTime) {
    simulation->FixedUpdate(deltaTime);
}

Simulation *Engine::GetSimulation()
{
    return simulation_;
}
