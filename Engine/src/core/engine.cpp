#include "core/engine.h"

#include <cmath>
#include <thread>

#include "simulation/simulation.h"

Engine::Engine() : simulation_(new Simulation()), engine_speed_(1.0) { }

Engine::~Engine() {
  delete simulation_;
}

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

    // time since last Update call
    auto update_delta =
        std::chrono::duration<double>(current_time - last_update_time_) *
        speed;

    if (update_delta.count() > 0.25)
      update_delta = std::chrono::duration<double>(0.25);

    last_update_time_ = current_time;
    time_since_fixed_update += update_delta;

    simulation_->Update(update_delta.count() * speed);

    while (time_since_fixed_update.count() >= kFixedUpdateInterval) {
      simulation_->FixedUpdate(kFixedUpdateInterval);
      time_since_fixed_update -= fixed_update_interval_duration;
    }

    std::this_thread::yield();
  }
}

void Engine::SetSpeed(double speed) { engine_speed_ = std::max(0.0, speed); }

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
