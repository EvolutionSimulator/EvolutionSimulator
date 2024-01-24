#include "simulationdata.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "collision_functions.h"
#include "food.h"
#include "mathlib.h"
#include "settings.h"

/*!
 * @brief Retrieves the current environment of the simulation.
 *
 * @return Returns the simulation's current environment.
 */

Environment SimulationData::GetEnvironment() { return environment_; }

/*!
 * @brief Sets the environment for the simulation.
 *
 * @param environment The new environment to be set for the simulation.
 */

void SimulationData::SetEnvironment(Environment& environment) {
  environment_ = environment;
}



void SimulationData::UpdateStatistics() {
  double current_time = world_time_;
  if (current_time - lastRecordedTime_ >= 1.0) {
    lastRecordedTime_ = current_time;
    creatureCountOverTime_.push_back(creatures_.size());
    double average_size = 0.0;
    double average_energy = 0.0;
    double average_velocity = 0.0;
    double average_diet = 0.0;
    double average_offspring = 0.0;
    for (const auto creature : creatures_) {
      average_size += creature->GetSize();
      average_energy += creature->GetEnergy();
      average_velocity += creature->GetVelocity();
      average_diet += creature->GetMutable().GetDiet();
      average_offspring += creature->GetOffspringNumber();
    }
    average_size /= creatures_.size();
    average_energy /= creatures_.size();
    average_velocity /= creatures_.size();
    average_diet /= creatures_.size();
    average_offspring /= creatures_.size();
    creatureSizeOverTime_.push_back(average_size);
    creatureEnergyOverTime_.push_back(average_energy);
    creatureVelocityOverTime_.push_back(average_velocity);
    creatureDietOverTime_.push_back(average_diet);
    creatureOffspringOverTime_.push_back(average_offspring);
  }
}

std::vector<int> SimulationData::GetCreatureCountOverTime() const {
  return creatureCountOverTime_;
}

std::vector<double> SimulationData::GetCreatureSizeOverTime() const {
  return creatureSizeOverTime_;
}

std::vector<double> SimulationData::GetCreatureEnergyOverTime() const {
  return creatureEnergyOverTime_;
}

std::vector<double> SimulationData::GetCreatureVelocityOverTime() const {
  return creatureVelocityOverTime_;
}

std::vector<double> SimulationData::GetCreatureDietOverTime() const {
  return creatureDietOverTime_;
}

std::vector<double> SimulationData::GetCreatureOffspringOverTime() const {
  return creatureOffspringOverTime_;
}

void SimulationData::WriteDataToFile() {
    // identifying the correct path for each platform
    std::string file_path;
    #ifdef _WIN32
        file_path = "C:\\Program Files\\EvolutionSimulator\\";
    #elif __APPLE__
        file_path = "~/Library/Application Support/EvolutionSimulator/";
    #elif __unix__
        file_path = "/.EvolutionSimulator/";
    #endif

    // getting the file count
    int n, simulation_count = 0;
    std::string file_number;
    for (const auto& simulation_saves : std::filesystem::directory_iterator(file_path)) {
        n = simulation_saves.path().stem().string().length();
        file_number = "";
        for (int i = 11; i < n-4; i++) {
            file_number += simulation_saves.path().stem().string()[i];
        }
        if (std::stoi(file_number) > simulation_count) {
            simulation_count = std::stoi(file_number);
        }
    }
    // writing the data to the new file
    std::ofstream WriteSimulation(file_path + "simulation_" + std::to_string(simulation_count));
    nlohmann::json simulation_json;

    // load simulation settings
    simulation_json["width"] = SimulationData::GetEnvironment().GetMapWidth();
    simulation_json["height"] = SimulationData::GetEnvironment().GetMapHeight();
    simulation_json["food density"] = SimulationData::GetEnvironment().GetFoodDensity(SimulationData::GetEnvironment().GetMapWidth(), SimulationData::GetEnvironment().GetMapHeight());
    simulation_json["creature density"] = SimulationData::GetEnvironment().GetCreatureDensity();

    // load the food from the current simulation
    // nlohmann::json food;
    simulation_json["food"] = nlohmann::json::array();
    for (const auto& food_item : SimulationData::food_entities_) {
        nlohmann::json food_entry;
        food_entry["type"] = food_item->GetType();
        food_entry["nutritional value"] = food_item->GetNutritionalValue();
        // food_entry["lifespan"] = food_item->GetLifespan();
        food_entry["size"] = food_item->GetSize();
        food_entry["orientation"] = food_item->GetOrientation();
        food_entry["state"] = food_item->GetState();
        food_entry["color"] = food_item->GetColor();

        simulation_json["food"] += food_entry;
    }

    // load the eggs from the current simulation
    simulation_json["eggs"] = nlohmann::json::array();
    for (const auto& egg_item : SimulationData::eggs_) {
        nlohmann::json egg_entry;
        egg_entry["incubation time"] = egg_item->GetIncubationTime();
        egg_entry["health"] = egg_item->GetHealth();
        egg_entry["age"] = egg_item->GetAge();

        // decompose the genome
        egg_entry["genome"]["links"] = nlohmann::json::array();
        for (const auto& link : egg_item->GetGenome().GetLinks()) {
            nlohmann::json link_entry;
            link_entry["id"] = link.GetId();
            link_entry["in"] = link.GetInId();
            link_entry["out"] = link.GetOutId();
            link_entry["weight"] = link.GetWeight();
            link_entry["active"] = link.IsActive();
            egg_entry["genome"]["links"] += link_entry;
        }

        egg_entry["genome"]["neurons"] = nlohmann::json::array();
        for (const auto& neuron : egg_item->GetGenome().GetNeurons()) {
            nlohmann::json neuron_entry;
            neuron_entry["id"] = neuron.GetId();
            neuron_entry["type"] = neuron.GetType();
            neuron_entry["bias"] = neuron.GetBias();
            egg_entry["genome"]["neurons"] += neuron_entry;
        }

        simulation_json["eggs"] += egg_entry;
    }

    // load the eggs from the current simulation
    simulation_json["creatures"] = nlohmann::json::array();
    for (const auto& creature_item : SimulationData::creatures_) {
        nlohmann::json creature_entry;
        creature_entry["health"] = creature_item->GetHealth();
        creature_entry["age"] = creature_item->GetAge();
        creature_entry["size"] = creature_item->GetSize();
        creature_entry["orientation"] = creature_item->GetOrientation();
        creature_entry["state"] = creature_item->GetState();
        creature_entry["color"] = creature_item->GetColor();
        creature_entry["energy"] = creature_item->GetEnergy();
        creature_entry["velocity"] = creature_item->GetVelocity();
        creature_entry["generation"] = creature_item->GetGeneration();

        // decompose the genome
        creature_entry["genome"]["links"] = nlohmann::json::array();
        for (const auto& link : creature_item->GetGenome().GetLinks()) {
            nlohmann::json link_entry;
            link_entry["id"] = link.GetId();
            link_entry["in"] = link.GetInId();
            link_entry["out"] = link.GetOutId();
            link_entry["weight"] = link.GetWeight();
            link_entry["active"] = link.IsActive();
            creature_entry["genome"]["links"] += link_entry;
        }

        creature_entry["genome"]["neurons"] = nlohmann::json::array();
        for (const auto& neuron : creature_item->GetGenome().GetNeurons()) {
            nlohmann::json neuron_entry;
            neuron_entry["id"] = neuron.GetId();
            neuron_entry["type"] = neuron.GetType();
            neuron_entry["bias"] = neuron.GetBias();
            creature_entry["genome"]["neurons"] += neuron_entry;
        }

        simulation_json["creatures"] += creature_entry;
    }
}

void SimulationData::RetrieveDataFromFile() {

}
