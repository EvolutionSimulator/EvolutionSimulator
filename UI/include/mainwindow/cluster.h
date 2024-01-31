#ifndef CLUSTER_H
#define CLUSTER_H

#include <unordered_map>
#include <mutex>

#include "entity/creature/creature.h"
#include "neat/genome.h"
#include "simulation/simulation.h"

struct CreatureData {
  neat::Genome genome;
  Mutable mut;
  bool alive;
  float hue;

  double distance(CreatureData other) const {
    return genome.CompatibilityBetweenGenomes(other.genome) +
           mut.CompatibilityBetweenMutables(other.mut);
  }
};

struct SpeciesData {
  int species_label;
  float species_color;
};

class Cluster {
 private:
  std::unordered_map<int, CreatureData>
      points;  //'keys' are creature ids, values are data points corresponding
               // to a creature
  std::unordered_map<int, int>
      species;  //'keys' are creature ids, values are data points corresponding to a species
  std::unordered_map<int, float>
      species_colors_; //'keys' are species ids, values are species color
  std::vector<int> core_points_ids;
  int next_species_label;
  double epsilon;
  int minPts;

  std::vector<std::tuple<int, double, int, float>> species_data_;

  volatile bool running_;
  double lastRecordedTime_;
  double lastReclusterTime_;

  std::recursive_mutex mutex_;

  void init(Simulation* simulation);

 public:
  Cluster(double epsilon, int minPts);

  void start(Simulation* simulation);
  void stop();

  void setPoints(const std::vector<std::shared_ptr<Creature>>& creatures);
  void run();
  void recluster();
  void add_newborns(const std::vector<std::shared_ptr<Creature>>& new_creatures);
  void update_dead_creatures(const std::vector<std::shared_ptr<Creature>>& dead_creatures);
  void update_all_creatures(const std::vector<std::shared_ptr<Creature>>& creatures);
  void update_creatures_species(
      std::vector<std::shared_ptr<Creature>>& creatures);

  std::vector<std::tuple<int, double, int, float>> getSpeciesData();

  std::unordered_map<int, int> getSpecies() const;
  std::unordered_map<int, int> speciesSizes();

  std::vector<std::tuple<int, double, int, float>> getCurrentSpeciesData();

  void CopyCreatures(const std::vector<std::shared_ptr<Creature>>& creatures);

 private:
  std::vector<int> GetNeighbors(int id);
  void expandCluster(int id, std::vector<int>& neighbors);

  std::vector<std::shared_ptr<Creature>> creatures_;

};

#endif // CLUSTER_H
