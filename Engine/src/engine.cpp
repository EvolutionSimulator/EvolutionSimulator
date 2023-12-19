#include "engine.h"

#include <cmath>

#include "simulation.h"

Engine::Engine() : environment_(), simulation_(new Simulation(environment_)) {}

Engine::Engine(double food_density, double creature_density) : environment_() {
  environment_.SetFoodDensity(food_density);
  environment_.SetCreatureDensity(creature_density);
  simulation_ = new Simulation(environment_);
}

Engine::~Engine() {
  //  delete simulation_;
}

myEnvironment::Environment& Engine::GetEnvironment() { return environment_; }

// Main engine loop
void Engine::Run() {
  if (running_) return;

  running_ = true;
  paused_ = false;

  engine_start_time_ = timer::now();
  last_update_time_ = engine_start_time_;
  last_fixed_update_time_ = engine_start_time_;

  simulation_->Start();

  while (running_) {
    if (paused_) continue;

    timer::time_point current_time = timer::now();

    // time since last FixedUpdate call
    double fixed_update_delta =
        std::chrono::duration<double>(current_time - last_fixed_update_time_)
            .count();

    // time since last Update call
    double update_delta =
        std::chrono::duration<double>(current_time - last_update_time_).count();

    // we calculate how many times we should call FixedUpdate using the time
    // since last execution
    int fixed_update_steps =
        std::floor(fixed_update_delta / kFixedUpdateInterval);
    for (int i = 0; i < fixed_update_steps; i++) {
      simulation_->FixedUpdate(kFixedUpdateInterval);
    }

    simulation_->Update(update_delta);

    last_update_time_ = current_time;
    // we increment lastFixedUpdateTime_ by the (update interval) * (number of
    // times we called it during this cycle)
    auto duration_to_add =
        std::chrono::duration_cast<timer::time_point::duration>(
            std::chrono::duration<double>(fixed_update_steps *
                                          kFixedUpdateInterval));
    last_fixed_update_time_ += duration_to_add;
  }
}

void Engine::UpdateEnvironment() {}

void Engine::Stop() { running_ = false; }

void Engine::Pause() { paused_ = true; }

void Engine::Resume()
{
  if (!paused_) return;

  timer::time_point current_time = timer::now();
  last_update_time_ = current_time;
  last_fixed_update_time_ = current_time;

  paused_ = false;
}

Simulation* Engine::GetSimulation() { return simulation_; }
