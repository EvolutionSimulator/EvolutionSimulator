#ifndef NEATNEURALNETWORK_H
#define NEATNEURALNETWORK_H

#endif  // NEATNEURALNETWORK_H

#include "neat/neat-genome.h"

namespace neat {

struct NeuronInput {
  int input_id;
  double weight;
};

struct FeedForwardNeuron {  // Functional units of the NeuralNetowork, obtained
                            // from Neurons and Links
  int id;
  double bias;
  std::vector<NeuronInput> inputs;
};

class NeuralNetwork {
 public:
  NeuralNetwork(const Genome &genom);
  std::vector<double> Activate(const std::vector<double> &input_values) const;

 private:
  std::vector<int> input_ids_;
  std::vector<int> output_ids_;
  std::vector<FeedForwardNeuron> ffneurons_;
};

std::vector<std::vector<Neuron> > get_layers(
    const Genome
        &genom);  // organize neurons from a Genome into layers (by neurons ids)

double activation_function(double x);

}  // end of namespace neat
