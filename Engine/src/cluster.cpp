#include "cluster.h"

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
    if (points[id].distance(pair.second) < epsilon) {
      neighbors.push_back(pair.first);
    }
  }

  return neighbors;
}

void Cluster::expandCluster(int id, std::vector<int>& neighbors,
                            int speciesLabel) {
  species[id] = speciesLabel;

  for (size_t i = 0; i < neighbors.size(); ++i) {
    int neighborId = neighbors[i];

    if (species.find(neighborId) != species.end()) {
      if (species[neighborId] == 0) {
        species[neighborId] =
            speciesLabel;  // Change noise point to border point
      }
    } else {
      species[neighborId] = speciesLabel;

      std::vector<int> newNeighbors = GetNeighbors(neighborId);

      if (newNeighbors.size() >= minPts) {
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
