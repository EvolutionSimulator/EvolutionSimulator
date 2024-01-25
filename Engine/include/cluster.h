#ifndef CLUSTER_H
#define CLUSTER_H

#endif // CLUSTER_H

#include <unordered_map>

#include "creature.h"
#include "neat/neat-genome.h"

struct CreatureData {
  neat::Genome genome;
  Mutable mut;
  bool alive;

  double distance(CreatureData other) const {
    return genome.CompatibilityBetweenGenomes(other.genome) +
           mut.CompatibilityBetweenMutables(other.mut);
  }
};

class Cluster {
 private:
  std::unordered_map<int, CreatureData> points;
  std::unordered_map<int, int> species;
  double epsilon;
  int minPts;

 public:
  Cluster(double epsilon, int minPts);
  void setPoints(const std::vector<Creature>& creatures);
  void run();
  std::unordered_map<int, int> getSpecies() const;

 private:
  std::vector<int> GetNeighbors(int id);
  void expandCluster(int id, std::vector<int>& neighbors, int speciesLabel);
};
