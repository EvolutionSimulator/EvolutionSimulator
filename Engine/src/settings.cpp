#include "settings.h"

#include <nlohmann/json.hpp>

void Settings::LoadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open config file: " << filename << std::endl;
    return;
  }

  nlohmann::json config_json;
  file >> config_json;

  // Load Neat settings
  auto& neat_json = config_json["neat"];
  neat.weight_mutation_rate = neat_json["weight_mutation_rate"].get<double>();
  neat.standard_deviation_weight = neat_json["standard_deviation_weight"].get<double>();
  neat.max_weight = neat_json["max_weight"].get<double>();
  neat.min_weight = neat_json["min_weight"].get<double>();
  neat.add_neuron_mutation_rate = neat_json["add_neuron_mutation_rate"].get<double>();
  neat.add_link_mutation_rate = neat_json["add_link_mutation_rate"].get<double>();
  neat.remove_neuron_mutation_rate = neat_json["remove_neuron_mutation_rate"].get<double>();
  neat.remove_link_mutation_rate = neat_json["remove_link_mutation_rate"].get<double>();
  neat.change_weight_mutation_rate = neat_json["change_weight_mutation_rate"].get<double>();
  neat.bias_mutation_rate = neat_json["bias_mutation_rate"].get<double>();
  neat.max_bias = neat_json["max_bias"].get<double>();
  neat.min_bias = neat_json["min_bias"].get<double>();
  neat.change_bias_mutation_rate = neat_json["change_bias_mutation_rate"].get<double>();
  neat.adjustment_probability = neat_json["adjustment_probability"].get<double>();

  // Load Environment settings
  auto& env_json = config_json["environment"];
  environment.map_width = env_json["map_width"].get<double>();
  environment.map_height = env_json["map_height"].get<double>();
  environment.creature_density = env_json["creature_density"].get<double>();
  environment.max_food_size = env_json["max_food_size"].get<int>();
  environment.max_creature_size = env_json["max_creature_size"].get<int>();
  environment.tolerance = env_json["tolerance"].get<double>();
  environment.default_food_density = env_json["default_food_density"].get<double>();
  environment.energy_to_health = env_json["energy_to_health"].get<double>();
  environment.health_to_energy = env_json["health_to_energy"].get<double>();
  environment.default_creature_density = env_json["default_creature_density"].get<double>();
  environment.plant_nutritional_value = env_json["plant_nutritional_value"].get<double>();
  environment.meat_nutritional_value = env_json["meat_nutritional_value"].get<double>();
  environment.rot_factor = env_json["rot_factor"].get<double>();
  environment.grid_cell_size = env_json["grid_cell_size"].get<double>();
  environment.min_creature_size = env_json["min_creature_size"].get<int>();
  environment.reproduction_threshold = env_json["reproduction_threshold"].get<double>();
  environment.reproduction_cooldown = env_json["reproduction_cooldown"].get<double>();
  environment.input_neurons = env_json["input_neurons"].get<int>();
  environment.output_neurons = env_json["output_neurons"].get<int>();
  environment.max_nutritional_value = env_json["max_nutritional_value"].get<double>();
  environment.default_lifespan = env_json["default_lifespan"].get<double>();
  environment.photosynthesis_factor = env_json["photosynthesis_factor"].get<double>();
  environment.frictional_coefficient = env_json["frictional_coefficient"].get<double>();

  // Load Engine settings
  auto& engine_json = config_json["engine"];
  engine.fixed_update_interval = engine_json["fixed_update_interval"].get<double>();
}
