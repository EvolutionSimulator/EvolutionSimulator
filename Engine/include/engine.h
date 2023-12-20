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
  void Pause();
  void Resume();
  void UpdateEnvironment();
  void SetSpeed(double speed);

  Simulation* GetSimulation();
  myEnvironment::Environment& GetEnvironment();

 private:
  myEnvironment::Environment environment_;
  const double kFixedUpdateInterval =
      SETTINGS.engine.fixed_update_interval;  // how often FixedUpdate is
                                              // called
  double engine_speed_ = 1.0;

  Simulation* simulation_;
  volatile bool running_ = false;
  volatile bool paused_ = false;

  typedef std::chrono::system_clock timer;
  timer::time_point engine_start_time_;
  timer::time_point last_update_time_;
  timer::time_point last_fixed_update_time_;
};
