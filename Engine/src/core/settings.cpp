#include "core/settings.h"

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
  neat.module_activation_mutation_rate = neat_json["module_activation_mutation_rate"].get<double>();
  neat.adjustment_probability = neat_json["adjustment_probability"].get<double>();
  neat.activation_function_mutation_rate = neat_json["activation_function_mutation_rate"].get<double>();

  // Load Compatibility settings
  auto& compatibility_json = config_json["compatibility"];
  compatibility.weight_shared_neurons = compatibility_json["weight_shared_neurons"].get<double>();
  compatibility.weight_shared_links = compatibility_json["weight_shared_links"].get<double>();
  compatibility.average_weight_shared_links = compatibility_json["average_weight_shared_links"].get<double>();
  compatibility.color_compatibility = compatibility_json["color_compatibility"].get<double>();
  compatibility.mutables_compatibility = compatibility_json["mutables_compatibility"].get<double>();
  compatibility.compatibility_threshold = compatibility_json["compatibility_threshold"].get<double>();
  compatibility.compatibility_distance = compatibility_json["compatibility_distance"].get<double>();

  // Load Environment settings
  auto& environment_json = config_json["environment"];
  environment.map_width = environment_json["map_width"].get<double>();
  environment.map_height = environment_json["map_height"].get<double>();
  environment.creature_density = environment_json["creature_density"].get<double>();
  environment.max_food_size = environment_json["max_food_size"].get<int>();
  environment.max_creature_size = environment_json["max_creature_size"].get<int>();
  environment.tolerance = environment_json["tolerance"].get<double>();
  environment.default_food_density = environment_json["default_food_density"].get<double>();
  environment.food_spawn_rate = environment_json["food_spawn_rate"].get<double>();
  environment.energy_to_health = environment_json["energy_to_health"].get<double>();
  environment.health_to_energy = environment_json["health_to_energy"].get<double>();
  environment.default_creature_density = environment_json["default_creature_density"].get<double>();
  environment.plant_nutritional_value = environment_json["plant_nutritional_value"].get<double>();
  environment.meat_nutritional_value = environment_json["meat_nutritional_value"].get<double>();
  environment.egg_nutritional_value = environment_json["egg_nutritional_value"].get<double>();
  environment.egg_incubation_time_multiplier = environment_json["egg_incubation_time_multiplier"].get<double>();
  environment.plant_proportion = environment_json["plant_proportion"].get<double>();
  environment.rot_factor = environment_json["rot_factor"].get<double>();
  environment.grid_cell_size = environment_json["grid_cell_size"].get<double>();
  environment.min_creature_size = environment_json["min_creature_size"].get<int>();
  environment.reproduction_threshold = environment_json["reproduction_threshold"].get<double>();
  environment.reproduction_cooldown = environment_json["reproduction_cooldown"].get<double>();
  environment.input_neurons = environment_json["input_neurons"].get<int>();
  environment.output_neurons = environment_json["output_neurons"].get<int>();
  environment.max_nutritional_value = environment_json["max_nutritional_value"].get<double>();
  environment.default_lifespan = environment_json["default_lifespan"].get<double>();
  environment.photosynthesis_factor = environment_json["photosynthesis_factor"].get<double>();
  environment.frictional_coefficient = environment_json["frictional_coefficient"].get<double>();
  environment.maturity_age_multiplier = environment_json["maturity_age_multiplier"].get<double>();
  environment.male_reproduction_cost = environment_json["male_reproduction_cost"].get<double>();
  environment.pregnancy_hardship_modifier = environment_json["pregnancy_hardship_modifier"].get<double>();
  environment.surface_dimension = environment_json["surface_dimension"].get<int>();
  environment.volume_dimension = environment_json["volume_dimension"].get<int>();
  environment.movement_energy = environment_json["movement_energy"].get<double>();
  environment.heat_energy = environment_json["heat_energy"].get<double>();

  // Load Engine settings
  auto& engine_json = config_json["engine"];
  engine.fixed_update_interval = engine_json["fixed_update_interval"].get<double>();
  engine.eps = engine_json["eps"].get<double>();
  engine.max_cells_to_find_food = engine_json["max_cells_to_find_food"].get<size_t>();
  engine.max_food_density_colored = engine_json["max_food_density_colored"].get<double>();

  // Load Physical Constraints settings
  auto& physical_constraints_json = config_json["physical_constraints"];
  physical_constraints.mutation_rate = physical_constraints_json["mutation_rate"].get<double>();
  physical_constraints.max_energy_density = physical_constraints_json["max_energy_density"].get<double>();
  physical_constraints.min_energy_loss = physical_constraints_json["min_energy_loss"].get<double>();
  physical_constraints.d_energy_density = physical_constraints_json["d_energy_density"].get<double>();
  physical_constraints.d_energy_loss = physical_constraints_json["d_energy_loss"].get<double>();
  physical_constraints.d_integrity = physical_constraints_json["d_integrity"].get<double>();
  physical_constraints.d_strafing_difficulty = physical_constraints_json["d_strafing_difficulty"].get<double>();
  physical_constraints.d_max_size = physical_constraints_json["d_max_size"].get<double>();
  physical_constraints.d_baby_size = physical_constraints_json["d_baby_size"].get<double>();
  physical_constraints.d_max_force = physical_constraints_json["d_max_force"].get<double>();
  physical_constraints.d_growth_factor = physical_constraints_json["d_growth_factor"].get<double>();
  physical_constraints.d_vision_factor = physical_constraints_json["d_vision_factor"].get<double>();
  physical_constraints.d_gestation_ratio_to_incubation = physical_constraints_json["d_gestation_ratio_to_incubation"].get<double>();
  physical_constraints.vision_radius = physical_constraints_json["vision_radius"].get<double>();
  physical_constraints.vision_angle = physical_constraints_json["vision_angle"].get<double>();
  physical_constraints.vision_ar_ratio = physical_constraints_json["vision_ar_ratio"].get<double>();
  physical_constraints.color_mutation_factor = physical_constraints_json["color_mutation_factor"].get<double>();
  physical_constraints.d_stomach_capacity = physical_constraints_json["d_stomach_capacity"].get<double>();
  physical_constraints.d_diet = physical_constraints_json["d_diet"].get<double>();
  physical_constraints.d_eating_cooldown = physical_constraints_json["d_eating_cooldown"].get<double>();
  physical_constraints.d_digestion_rate = physical_constraints_json["d_digestion_rate"].get<double>();
  physical_constraints.d_genetic_strength = physical_constraints_json["d_genetic_strength"].get<double>();
  physical_constraints.d_acid_to_energy = physical_constraints_json["d_acid_to_energy"].get<double>();
  physical_constraints.max_reproducing_age = physical_constraints_json["max_reproducing_age"].get<double>();
  physical_constraints.mating_desire_max_prob = physical_constraints_json["mating_desire_max_prob"].get<double>();
  physical_constraints.mating_desire_factor = physical_constraints_json["mating_desire_factor"].get<double>();
  physical_constraints.pregnancy_energy_factor = physical_constraints_json["pregnancy_energy_factor"].get<double>();
  physical_constraints.pregnancy_velocity_factor = physical_constraints_json["pregnancy_velocity_factor"].get<double>();
  physical_constraints.after_birth_velocity_factor = physical_constraints_json["after_birth_velocity_factor"].get<double>();
  physical_constraints.d_bite_damage_ratio = physical_constraints_json["d_bite_damage_ratio"].get<double>();
  physical_constraints.d_bite_energy_consumption_ratio = physical_constraints_json["d_bite_energy_consumption_ratio"].get<double>();
  physical_constraints.d_bite_nutritional_value = physical_constraints_json["d_bite_nutritional_value"].get<double>();
  physical_constraints.d_eating_speed = physical_constraints_json["d_eating_speed"].get<double>();
  physical_constraints.d_pheromone_emission = physical_constraints_json["d_pheromone_emission"].get<double>();
  physical_constraints.pheromone_detection_sensitivity = physical_constraints_json["pheromone_detection_sensitivity"].get<double>();
  physical_constraints.pheromone_emission_rate = physical_constraints_json["pheromone_emission_rate"].get<double>();

  // Load UI settings
  auto& ui_json = config_json["ui"];
  ui.dragging_sensitivity = ui_json["dragging_sensitivity"].get<double>();
  ui.min_zoom = ui_json["min_zoom"].get<double>();
  ui.max_zoom = ui_json["max_zoom"].get<double>();

  // Load Random settings
  auto& random_json = config_json["random"];
  random.seed = random_json["seed"].get<unsigned int>();
  random.input_seed = random_json["input_seed"].get<bool>();

  file.close();
}
