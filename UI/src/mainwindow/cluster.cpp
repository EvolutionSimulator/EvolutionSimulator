#include "mainwindow/cluster.h"

Cluster::Cluster(double epsilon, int minPts)
    : epsilon(epsilon), minPts(minPts) {}

void Cluster::setPoints(const std::vector<Creature>& creatures) {
  for (const Creature& creature : creatures) {
    points[creature.GetID()] =
        CreatureData{creature.GetGenome(), creature.GetMutable(), true};
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

void Cluster::add_newborns(const std::vector<Creature>& new_creatures) {
  for (const Creature& creature : new_creatures) {
    CreatureData creature_data{creature.GetGenome(), creature.GetMutable(),
                               true};
    points[creature.GetID()] = creature_data;
    // maybe core points ids should be shuffled every time
    bool assigned_species = false;
    for (const int& core_id : core_points_ids) {
      if (points[core_id].distance(creature_data) <= epsilon) {
        species[creature.GetID()] = species[core_id];
        assigned_species = true;
        break;
      }
    }
    if (!assigned_species) {
      species[creature.GetID()] = 0;  // noise
    }
  }
}

void Cluster::update_dead_creatures(
    const std::vector<Creature>& dead_creatures) {
  for (const Creature& creature : dead_creatures) {
    if (points.find(creature.GetID()) != points.end()) {
      points[creature.GetID()].alive = false;
    }
  }
}
