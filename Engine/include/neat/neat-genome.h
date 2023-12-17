#ifndef NEATGENOME_H
#define NEATGENOME_H

#include <random>
#include <unordered_set>
#include <vector>

#include "config.h"
#include "neat-link.h"
#include "neat-neuron.h"

namespace neat {

class Genome {
 public:
  explicit Genome(int input_count, int output_count);
  int GetInputCount() const;
  int GetOutputCount() const;
  const std::vector<Neuron>& GetNeurons() const;
  const std::vector<Link>& GetLinks() const;

  void AddNeuron(const Neuron& neuron);
  void AddLink(const Link& link);

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
  void MutateChangeBias();

  bool DetectLoops(const Neuron& n);

  bool HasLink(const int& in_id, const int& ou_id);

 private:
  std::vector<Neuron> neurons_; /*!< A vector of Neuron objects representing the neurons in the Genome. */
  std::vector<Link> links_; /*!< A vector of Link objects representing the connections between neurons in the Genome. */
  bool DFS(const Neuron& currentNeuron, std::unordered_set<int>& visited, std::unordered_set<int>& visiting) const;
};

Genome Crossover(const Genome &dominant, const Genome &recessive);
}  // namespace neat

#endif  // NEATGENOME_H
