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
  std::unordered_map<int, CreatureData>
      points;  //'keys' are creature ids, values are data points corresponding
               // to a creature
  std::unordered_map<int, int>
      species;  //'keys' are creature ids, values are species labels
  std::vector<int> core_points_ids;
  double epsilon;
  int minPts;

 public:
  Cluster(double epsilon, int minPts);

  void setPoints(const std::vector<Creature>& creatures);
  void run();
  void add_newborns(const std::vector<Creature>& new_creatures);
  void update_dead_creatures(const std::vector<Creature>& dead_creatures);

  std::unordered_map<int, int> getSpecies() const;
  std::unordered_map<int, int> speciesSizes();

 private:
  std::vector<int> GetNeighbors(int id);
  void expandCluster(int id, std::vector<int>& neighbors, int speciesLabel);
};
