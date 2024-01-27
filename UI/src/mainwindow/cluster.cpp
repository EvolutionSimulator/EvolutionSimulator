#include "mainwindow/cluster.h"

#include <thread>

Cluster::Cluster(double epsilon, int minPts)
    : epsilon(epsilon),
      minPts(minPts),
      running_(false),
      lastRecordedTime_(0.0),
      species(),
      points() {}

void Cluster::start(Simulation* simulation) {
  running_ = false;
  init(simulation);
  run();

  while (running_) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto data = simulation->GetSimulationData();

    // std::cout << "Simulation time: " << data->world_time_ << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    update_all_creatures(data->creatures_);
    lastRecordedTime_ = data->world_time_;
  }
}

void Cluster::init(Simulation* simulation) {
  // std::cout << "Initializing cluster" << std::endl;
  auto data = simulation->GetSimulationData();
  setPoints(data->creatures_);
}

void Cluster::stop() {
  running_ = false;
  species.clear();
  core_points_ids.clear();
}

void Cluster::setPoints(const std::vector<std::shared_ptr<Creature>>& creatures) {
  for (const std::shared_ptr<Creature>& creature : creatures) {
    points[creature->GetID()] =
        CreatureData{creature->GetGenome(), creature->GetMutable(), true};
  }
}

std::vector<int> Cluster::GetNeighbors(int id) {
  std::vector<int> neighbors;

  for (const auto& pair : points) {
    if (points[id].distance(pair.second) <= epsilon) {
      neighbors.push_back(pair.first);
    }
  }

  return neighbors;
}

void Cluster::expandCluster(int id, std::vector<int>& neighbors,
                            int speciesLabel) {
  species[id] = speciesLabel;
  core_points_ids.push_back(id);
  for (size_t i = 0; i < neighbors.size(); ++i) {
    int neighborId = neighbors[i];

    if (species.find(neighborId) != species.end()) {
      if (species[neighborId] == 0) {
        species[neighborId] =
            speciesLabel;  // Change noise point to border point
      }
      // else this point already belongs to a different species, so do nothing
    } else {  // point is not labeled
      species[neighborId] = speciesLabel;

      std::vector<int> newNeighbors = GetNeighbors(neighborId);

      if (newNeighbors.size() >= minPts) {
        core_points_ids.push_back(neighborId);
        neighbors.insert(neighbors.end(), newNeighbors.begin(),
                         newNeighbors.end());
      }
    }
  }  // end of for loop
}

void Cluster::run() {
  // std::cout << "Running cluster" << std::endl;
  std::lock_guard<std::mutex> lock(mutex_);

  int speciesLabel = 0;

  for (const auto& pair : points) {
    if (species.find(pair.first) != species.end())
      continue;  // Point already processed

    std::vector<int> neighbors = GetNeighbors(pair.first);

    if (neighbors.size() < minPts) {
      species[pair.first] = 0;  // Mark as noise
    } else {
      expandCluster(pair.first, neighbors, ++speciesLabel);
    }
  }
}

std::unordered_map<int, int> Cluster::getSpecies() const { return species; }

std::unordered_map<int, int> Cluster::speciesSizes() {
  std::unordered_map<int, int> species_sizes;
  for (const auto& pair : species) {
    if (!points[pair.first].alive) {
      continue;
    }
    if (species_sizes.find(pair.second) == species_sizes.end()) {
      species_sizes[pair.second] = 1;
    } else {
      species_sizes[pair.second]++;
    }
  }
  return species_sizes;
}

std::vector<std::tuple<double, int, double>> Cluster::getSpeciesData() {
  std::lock_guard<std::mutex> lock(mutex_);

  std::unordered_map<int, int> species_sizes = this->speciesSizes();
  std::vector<std::tuple<double, int, double>> species_data;

  for (const auto& pair : species_sizes) {
    species_data.push_back(
        std::make_tuple((double)pair.first, pair.second, lastRecordedTime_));
  }

  return species_data;
}

void Cluster::add_newborns(const std::vector<std::shared_ptr<Creature>>& new_creatures) {
  for (const std::shared_ptr<Creature>& creature : new_creatures) {
    CreatureData creature_data{creature->GetGenome(), creature->GetMutable(),
                               true};
    points[creature->GetID()] = creature_data;
    // maybe core points ids should be shuffled every time
    bool assigned_species = false;
    for (const int& core_id : core_points_ids) {
      if (points[core_id].distance(creature_data) <= epsilon) {
        species[creature->GetID()] = species[core_id];
        assigned_species = true;
        break;
      }
    }
    if (!assigned_species) {
      species[creature->GetID()] = 0;  // noise
    }
  }
}

void Cluster::update_dead_creatures(
    const std::vector<std::shared_ptr<Creature>>& dead_creatures) {
  for (const std::shared_ptr<Creature>& creature : dead_creatures) {
    if (points.find(creature->GetID()) != points.end()) {
      points[creature->GetID()].alive = false;
    }
  }
}

void Cluster::update_all_creatures(const std::vector<std::shared_ptr<Creature>>& creatures) {
  // std::cout << "Updating cluster" << std::endl;

  for (auto& pair : points) {
    pair.second.alive = false;
  }

  for (const std::shared_ptr<Creature>& creature : creatures) {
    if (points.find(creature->GetID()) != points.end()) {
      points[creature->GetID()].alive = true;
    }
  }

  for (const std::shared_ptr<Creature>& creature : creatures) {
    if (points.find(creature->GetID()) != points.end()) {
      continue;
    }

    CreatureData creature_data{creature->GetGenome(), creature->GetMutable(),
                               true};
    points[creature->GetID()] = creature_data;
    // maybe core points ids should be shuffled every time
    bool assigned_species = false;
    for (const int& core_id : core_points_ids) {
      if (points[core_id].distance(creature_data) <= epsilon) {
        species[creature->GetID()] = species[core_id];
        assigned_species = true;
        break;
      }
    }
    if (!assigned_species) {
      species[creature->GetID()] = 0;  // noise
    }
  }
}
