#pragma once

struct Settings {
 public:
  // Singleton access method
  static Settings& GetInstance() {
    static Settings instance;
    return instance;
  }

  // Delete copy constructor and assignment operator
  Settings(const Settings&) = delete;
  Settings& operator=(const Settings&) = delete;

  struct NeatSettings {
    double weight_mutation_rate = 0.2;
    double standard_deviation_weight = 0.1;
    double max_weight = 1.0;
    double min_weight = 0.0;
    double add_neuron_mutation_rate = 0.05;
    double add_link_mutation_rate = 0.25;
    double remove_neuron_mutation_rate = 0.02;
    double remove_link_mutation_rate = 0.15;
    double change_weight_mutation_rate = 0.6;
    double bias_mutation_rate = 0.2;
    double max_bias = 0.1;
    double min_bias = 0.0;
    double change_bias_mutation_rate = 0.6;
    double adjustment_probability = 0.8;
  };

  struct EnvironmentSettings {
    double map_width = 1000.0;
    double map_height = 450.0;
    double creature_density = 0.0005;
    int max_food_size = 15;
    int max_creature_size = 15;
    double tolerance = 1e-3;
    double default_food_density = 0.005;
    double energy_to_health = 70.0;
    double health_to_energy = 10.0;
    double default_creature_density = 0.0001;
    double plant_nutritional_value = 2.0;
    double meat_nutritional_value = 3.0;
    double rot_factor = 1.0;
    double grid_cell_size = 50.0;
    int min_creature_size = 2;
    double reproduction_threshold = 0.90;
    double reproduction_cooldown = 10;
    int input_neurons = 6;
    int output_neurons = 3;
    double max_nutritional_value = 5;
    double default_lifespan = 30;
    double photosynthesis_factor = 0.1;
    double frictional_coefficient = 0.5;
  };

  struct EngineSettings {
    double fixed_update_interval = 0.05;
  };

  NeatSettings neat;
  EnvironmentSettings environment;
  EngineSettings engine;

 private:
  Settings() {}
};

#define SETTINGS Settings::GetInstance()
