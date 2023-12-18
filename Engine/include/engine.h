#pragma once

#include <chrono>

#include "environment.h"
#include "simulation.h"

class Simulation;

class Engine {
 public:
  Engine();
  Engine(double food_density, double creature_density);
  ~Engine();

  void Run();
  void Stop();
  void UpdateEnvironment();

  Simulation* GetSimulation();
  myEnvironment::Environment& GetEnvironment();

 private:
  myEnvironment::Environment environment_;
  const double kFixedUpdateInterval =
      settings::engine::kFixedUpdateInterval;  // how often FixedUpdate is
                                               // called
  Simulation* simulation_;
  bool running_ = false;

  typedef std::chrono::system_clock timer;
  timer::time_point engine_start_time_;
  timer::time_point last_update_time_;
  timer::time_point last_fixed_update_time_;
};
