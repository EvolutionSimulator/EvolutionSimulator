#include "core/engine.h"

#include <cmath>
#include <thread>

#include "simulation/simulation.h"
#include "core/random.h"

Engine::Engine(double width, double height)
    : environment_(width, height), simulation_(new Simulation(environment_)), engine_speed_(1.0) {

    if (!SETTINGS.random.input_seed){
        std::random_device rd;
        SETTINGS.random.seed = rd();
        std::cout << "Seed of the simulation: " << SETTINGS.random.seed << "\n";
    }
    Random::SetSeed(SETTINGS.random.seed);

}

Engine::Engine(double width, double height, double food_density,
               double creature_density)
    : environment_(width, height), simulation_(new Simulation(environment_)), engine_speed_(1.0) {
  environment_.SetFoodDensity(food_density);
  environment_.SetCreatureDensity(creature_density);
}

Engine::~Engine() { delete simulation_; }

Environment& Engine::GetEnvironment() { return environment_; }

// Main engine loop
void Engine::Run() {
  if (running_)
    return;

  running_ = true;
  // paused_ = false;

  engine_start_time_ = timer::now();
  last_update_time_ = engine_start_time_;
  last_fixed_update_time_ = engine_start_time_;
  auto time_since_fixed_update = std::chrono::duration<double>(0);
  auto fixed_update_interval_duration = std::chrono::duration<double>(kFixedUpdateInterval);

  simulation_->Start();
  while (running_) {
    if (paused_)
      continue;
    timer::time_point current_time = timer::now();
    double speed = engine_speed_;

    auto update_delta =
        std::chrono::duration<double>(current_time - last_update_time_) *
        speed;

    if (update_delta.count() > 0.05)
      update_delta = std::chrono::duration<double>(0.05);

    last_update_time_ = current_time;
    time_since_fixed_update += update_delta;

    simulation_->Update(update_delta.count());
    //simulation_ -> UpdateStatistics();

    while (time_since_fixed_update.count() >= kFixedUpdateInterval) {
      simulation_->FixedUpdate(kFixedUpdateInterval);
      time_since_fixed_update -= fixed_update_interval_duration;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }
}

void Engine::UpdateEnvironment() {}

bool Engine::IsPaused() { return paused_; }

void Engine::SetSpeed(double speed) { engine_speed_ = std::max(0.0, speed); }

double Engine::GetSpeed()
{
  return engine_speed_;
}

void Engine::Stop() { running_ = false; }

void Engine::Pause() { paused_ = true; }

void Engine::Resume() {
  if (!paused_)
    return;

  timer::time_point current_time = timer::now();
  last_update_time_ = current_time;
  last_fixed_update_time_ = current_time;

  paused_ = false;
}

Simulation *Engine::GetSimulation() { return simulation_; }
