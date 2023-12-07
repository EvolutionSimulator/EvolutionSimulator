#ifndef NEATGENOME_H
#define NEATGENOME_H

#include <vector>
#include <random>

#include "neat-link.h"
#include "neat-neuron.h"
#include <unordered_set>

namespace neat {

static double weightMutationRate = 0.2;
static double standardDeviationWeight = 0.1;
static double maxWeight = 1.0;
static double minWeight = 0.0;

static double addNeuronMutationRate = 0.05;
static double addLinkMutationRate = 0.25;
static double removeNeuronMutationRate = 0.02;
static double removeLinkMutationRate = 0.15;
static double changeWeightMutationRate = 0.6;

class Genome {
 public:
  explicit Genome(int input_count, int output_count);
  int GetInputCount() const;
  int GetOutputCount() const;
  const std::vector<Neuron> &GetNeurons() const;
  const std::vector<Link> &GetLinks() const;

  void AddNeuron(const Neuron &neuron);
  void AddLink(const Link &link);


  void DisableNeuron(int id); //don't use this for now
  void DisableLink(int id);
  void EnableNeuron(int id); //don't use this for now

  void EnableLink(int id);
  void RemoveNeuron(int id);
  void RemoveLink(int id);

  void Mutate();

  void MutateAddNeuron();
  void MutateAddLink();
  void MutateRemoveNeuron();
  void MutateRemoveLink();
  void MutateChangeWeight();

  bool DetectLoops(const Neuron& n);

  bool HasLink(const int& in_id, const int& ou_id);

 private:
  std::vector<Neuron> neurons_;
  std::vector<Link> links_;
  bool DFS(const Neuron& currentNeuron, std::unordered_set<int>& visited, std::unordered_set<int>& visiting) const;
};

Genome Crossover(const Genome &dominant, const Genome &recessive);
}  // namespace neat

#endif  // NEATGENOME_H
