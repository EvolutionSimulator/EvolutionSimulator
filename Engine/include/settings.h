#pragma once

#include <fstream>
#include <iostream>
#include <cmath>  // Added for M_PI

class Settings {
 public:
  // Singleton access method
  static Settings& GetInstance() {
    static Settings instance;
    return instance;
  }

  void LoadFromFile(const std::string& filename);

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
    double activation_function_mutation_rate = 0.02;
  } neat;

  struct CompatibilitySettings {
    double weight_shared_neurons = 0.2;
    double weight_shared_links = 0.3;
    double average_weight_shared_links = 0.5;
    double color_compatibility = 0.1;
    double mutables_compatibility = 0.5;
    double compatibility_threshold = 2.0;
  } compatibility;

  struct EnvironmentSettings {
    double map_width = 1900.0;
    double map_height = 880.0;
    double creature_density = 0.0005;
    int max_food_size = 15; // Influences vision
    int max_creature_size = 15;
    double tolerance = 1e-3;
    double default_food_density = 0.005;
    double food_spawn_rate = 5e-5;
    double energy_to_health = 70.0;
    double health_to_energy = 10.0;
    double default_creature_density = 0.0001;
    double plant_nutritional_value = 2.0;
    double meat_nutritional_value = 3.0;
    double plant_proportion = 0.5; // PlantProportion + MeatProportion = 1
    double rot_factor = 1.0;
    double grid_cell_size = 50.0;
    int min_creature_size = 2;
    double reproduction_threshold = 0.80;
    double reproduction_cooldown = 10;
    int input_neurons = 12;
    int output_neurons = 6;
    double max_nutritional_value = 5;
    double default_lifespan = 30;
    double photosynthesis_factor = 0.1;
    double frictional_coefficient = 0.15; // Needs to be less than 1
  } environment;

  struct EngineSettings {
    double fixed_update_interval = 0.05;
    double eps = 1e-7;
    size_t max_cells_to_find_food = 30;
  } engine;

  struct PhysicalConstraintsSettings {
    double mutation_rate = 0.2;
    double max_energy_density = 10.0;
    double min_energy_loss = 0.1;
    double d_energy_density = 5.0;
    double d_energy_loss = 0.5;
    double d_integrity = 3.0;
    double d_strafing_difficulty = 0.5;
    double d_max_size = 10;
    double d_baby_size = 2;
    double d_max_force = 10;
    double d_growth_factor = 10;
    double d_vision_factor = 200;
    double vision_radius = 200; // Default values or not used, not sure
    double vision_angle = M_PI / 3; // Default values or not used, not sure
    double vision_ar_ratio = 200 * M_PI / 3;
    double color_mutation_factor = 0.05;
    double d_stomach_capacity = 2;
    double d_diet = 0.5; // Default omnivores
    double d_eating_cooldown = 1; // Decrease for easier survival
    double d_digestion_rate = 3;
    double d_genetic_strength = 0.6;
    double d_acid_to_energy = 5; // Increase for easier survival
  } physical_constraints;

  struct UISettings {
    double dragging_sensitivity = 1.0f;
  } ui;

 private:
  Settings() {}
};

#define SETTINGS Settings::GetInstance()
