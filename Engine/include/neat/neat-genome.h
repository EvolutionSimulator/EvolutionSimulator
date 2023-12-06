#ifndef NEATGENOME_H
#define NEATGENOME_H

#include <vector>
#include <random>

#include "neat-link.h"
#include "neat-neuron.h"

namespace neat {

static double mutation_rate = 0.2; // TODO: give a value to this

class Genome {
 public:
  Genome(int input_count, int output_count);
  int GetInputCount() const;
  int GetOutputCount() const;
  const std::vector<Neuron> &GetNeurons() const;
  const std::vector<Link> &GetLinks() const;

  void AddNeuron(NeuronType type, double bias);
  void AddLink(int in_id, int out_id, double weight);

  void DisableNeuron(int id);
  void DisableLink(int id);
  void EnableNeuron(int id);
  void EnableLink(int id);
  void RemoveNeuron(int id);
  void RemoveLink(int id);

  void MutateAddNeuron();
  void MutateAddLink();
  void MutateRemoveNeuron();
  void MutateRemoveLink();
  void MutateChangeWeight();

 private:
  int input_count_;
  int output_count_;
  int next_neuron_id_;
  int next_link_id_;
  std::vector<Neuron> neurons_;
  std::vector<Link> links_;
};

}  // namespace neat

#endif  // NEATGENOME_H
