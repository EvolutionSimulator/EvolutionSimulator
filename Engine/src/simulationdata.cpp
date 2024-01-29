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

#include <QDebug>

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
    #include <fstream>
    #include <filesystem>

    // identifying the correct path for each platform
    std::filesystem::path currentFilePath = __FILE__;
    std::filesystem::path evolutionSimulatorPath = currentFilePath.parent_path().parent_path().parent_path();
    std::filesystem::path file_path = evolutionSimulatorPath / "Simulations";


    std::filesystem::create_directories(file_path);

    // getting the file count
    int n, simulation_count = 0;
    std::string file_number;
    for (const auto& simulation_saves : std::filesystem::directory_iterator(file_path)) {
        n = simulation_saves.path().stem().string().length();
        file_number = "";
        for (int i = 11; i < n; i++) {
            file_number += simulation_saves.path().stem().string()[i];
        }
        // qDebug() << file_number;
        if (std::stoi(file_number) > simulation_count) {
            simulation_count = std::stoi(file_number);
        }
    }
    // writing the data to the new file
    std::filesystem::path simulationFilePath = file_path / ("simulation_" + std::to_string(simulation_count + 1) + ".json");
    std::ofstream WriteSimulation(simulationFilePath);
    nlohmann::json simulation_json;

    // load simulation settings
    simulation_json["width"] = SETTINGS.environment.map_width;
    simulation_json["height"] = SETTINGS.environment.map_height;
    simulation_json["food density"] = SimulationData::GetEnvironment().GetFoodDensity(SETTINGS.environment.map_width,  SETTINGS.environment.map_height);
    simulation_json["creature density"] = SimulationData::GetEnvironment().GetCreatureDensity();

    // load the food from the current simulation
    // nlohmann::json food;
    simulation_json["food"] = nlohmann::json::array();
    for (const auto& food_item : SimulationData::food_entities_) {
        nlohmann::json food_entry;
        food_entry["x_coord"] = food_item->GetCoordinates().first;
        food_entry["y_coord"] = food_item->GetCoordinates().second;
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
        egg_entry["generation"] = egg_item->GetGeneration();
        egg_entry["x_coord"] = egg_item->Entity::GetCoordinates().first;
        egg_entry["y_coord"] = egg_item->Entity::GetCoordinates().second;

        // decompose the genome
        egg_entry["genome"]["in_count"] = egg_item->GetGenome().GetInputCount();
        egg_entry["genome"]["out_count"] = egg_item->GetGenome().GetOutputCount();
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
        creature_entry["x_coord"] = creature_item->GetCoordinates().first;
        creature_entry["y_coord"] = creature_item->GetCoordinates().second;
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
        creature_entry["genome"]["in_count"] = creature_item->GetGenome().GetInputCount();
        creature_entry["genome"]["out_count"] = creature_item->GetGenome().GetOutputCount();
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
        creature_entry["genome"]["in_count"] = creature_item->GetGenome().GetInputCount();
        creature_entry["genome"]["out_count"] = creature_item->GetGenome().GetOutputCount();
        for (const auto& neuron : creature_item->GetGenome().GetNeurons()) {
            nlohmann::json neuron_entry;
            neuron_entry["id"] = neuron.GetId();
            neuron_entry["type"] = neuron.GetType();
            neuron_entry["bias"] = neuron.GetBias();
            creature_entry["genome"]["neurons"] += neuron_entry;
        }

        simulation_json["creatures"] += creature_entry;
    }
    WriteSimulation << simulation_json.dump(4);
}

void SimulationData::RetrieveDataFromFile(const int& simulationNumber) {
    // identifying the correct path for each platform
    std::filesystem::path currentFilePath = __FILE__;
    std::filesystem::path evolutionSimulatorPath = currentFilePath.parent_path().parent_path().parent_path();
    std::filesystem::path file_path = evolutionSimulatorPath / "Simulations";

    // reading the data from the file
    std::filesystem::path simulationFilePath = file_path / ("simulation_" + std::to_string(simulationNumber) + ".json");
    std::ifstream ReadSimulation(simulationFilePath);
    nlohmann::json simulation_json;
    if (ReadSimulation.is_open()) {
        ReadSimulation >> simulation_json;
    } else {
        std::cerr << "Failed to open file for reading: " << simulationFilePath << std::endl;
    }

    // Delete old data
    SimulationData::creatures_.clear();
    SimulationData::food_entities_.clear();
    SimulationData::eggs_.clear();

    // load simulation settings
    Environment environment;
    SETTINGS.environment.map_width = simulation_json["width"];
    SETTINGS.environment.map_height = simulation_json["height"];

    environment.SetFoodDensity(simulation_json["food density"]);
    environment.SetCreatureDensity(simulation_json["creature density"]);
    SimulationData::SetEnvironment(environment);

    // load the food from the current simulation
    for (const auto& food_item : simulation_json["food"]) {
        double nutritional_value = food_item["nutritional value"];
        double x = food_item["x_coord"];
        double y = food_item["y_coord"];
        Food::type type = food_item["type"];
        if (type == Food::plant) {
            std::shared_ptr<Plant> food = std::make_shared<Plant>(x, y, nutritional_value);
            food->SetSize(food_item["size"]);
            food->SetOrientation(food_item["orientation"]);
            food->SetState(food_item["state"]);
            food->SetColor(food_item["color"]);
            SimulationData::food_entities_.push_back(food);
        }
        else {
            std::shared_ptr<Meat> food = std::make_shared<Meat>(x, y);
            food->SetSize(food_item["size"]);
            food->SetOrientation(food_item["orientation"]);
            food->SetState(food_item["state"]);
            food->SetColor(food_item["color"]);
            SimulationData::food_entities_.push_back(food);
        }
    }

    // load the eggs from the current simulation
    if (!simulation_json["eggs"].empty()) {
        for (const auto& egg_item : simulation_json["eggs"]) {
            auto coords = std::make_pair(egg_item["x_coord"], egg_item["y_coord"]);

            // reconstruct the genome
            neat::Genome genome = neat::Genome(egg_item["genome"]["in_count"], egg_item["genome"]["out_count"]);
            for (const auto& link : egg_item["genome"]["links"]) {
                neat::Link new_link = neat::Link(link["id"], link["in"], link["out"]);
                new_link.SetWeight(link["weight"]);
                if (link["active"] == true)
                    new_link.SetActive();
                genome.AddLink(new_link);
            }

            for (const auto& neuron : egg_item["genome"]["neurons"]) {
                neat::Neuron new_neuron = neat::Neuron(neuron["type"], neuron["bias"]);
                // new_neuron.id_ = neuron["id"];
                genome.AddNeuron(new_neuron);
            }
            std::shared_ptr<Egg> egg = std::make_shared<Egg>(GestatingEgg(genome, Mutable(), egg_item["generation"]), coords);
            // egg->SetIncubationTime(egg_item["incubation time"]);
            egg->SetHealth(egg_item["health"]);
            egg->SetAge(egg_item["age"]);
            //egg->SetGenome(genome);
            SimulationData::eggs_.push_back(egg);
        }
    }
    qDebug() << "Done Loading Eggs";
    // load the creatures into the current simulation
    for (const auto& creature_item : simulation_json["creatures"]) {
        auto coords = std::make_pair(creature_item["x_coord"], creature_item["y_coord"]);

        // reconstruct the genome
        neat::Genome genome = neat::Genome(creature_item["genome"]["in_count"], creature_item["genome"]["out_count"]);

        for (const auto& link : creature_item["genome"]["links"]) {
            neat::Link new_link = neat::Link(link["id"], link["in"], link["out"]);
            new_link.SetWeight(link["weight"]);
            if (link["active"] == true)
                new_link.SetActive();
            genome.AddLink(new_link);
        }

        for (const auto& neuron : creature_item["genome"]["neurons"]) {
            neat::Neuron new_neuron = neat::Neuron(neuron["type"], neuron["bias"]);
            // new_neuron.id_ = neuron["id"];
            genome.AddNeuron(new_neuron);
        }


        std::shared_ptr<Creature> creature = std::make_shared<Creature>(genome, Mutable());
        creature->SetHealth(creature_item["health"]);
        creature->SetAge(creature_item["age"]);
        creature->SetSize(creature_item["size"]);
        creature->SetOrientation(creature_item["orientation"]);
        creature->SetState(creature_item["state"]);
        creature->SetColor(creature_item["color"]);
        creature->SetEnergy(creature_item["energy"]);
        creature->SetVelocity(creature_item["velocity"]);
        creature->SetGeneration(creature_item["generation"]);
        SimulationData::creatures_.push_back(creature);
    }
    qDebug() << "Done Loading Creature";
}

void SimulationData::RetrieveLastSimulation() {
    // identifying the correct path for each platform
    std::filesystem::path currentFilePath = __FILE__;
    std::filesystem::path evolutionSimulatorPath = currentFilePath.parent_path().parent_path().parent_path();
    std::filesystem::path file_path = evolutionSimulatorPath / "Simulations";

    // getting the file count
    int n, simulation_count = 0;
    std::string file_number;
    for (const auto& simulation_saves : std::filesystem::directory_iterator(file_path)) {
        n = simulation_saves.path().stem().string().length();
        file_number = "";
        for (int i = 11; i < n; i++) {
            file_number += simulation_saves.path().stem().string()[i];
        }
        if (std::stoi(file_number) > simulation_count) {
            // qDebug() << file_number;
            simulation_count = std::stoi(file_number);
        }
    }

    // call the function to retrieve the data from the file
    SimulationData::RetrieveDataFromFile(simulation_count);
}
