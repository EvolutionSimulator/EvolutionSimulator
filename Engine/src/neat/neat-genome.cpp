#include "neat/neat-genome.h"

namespace neat {

Genome::Genome(int input_count, int output_count)
    : input_count_(input_count),
      output_count_(output_count),
      next_neuron_id_(0),
      next_link_id_(0),
      neurons_(),
      links_() {
  for (int i = 0; i < input_count_; ++i) {
    AddNeuron(NeuronType::kInput, 0.0);
  }
  for (int i = 0; i < output_count_; ++i) {
    AddNeuron(NeuronType::kOutput, 0.0);
  }
}

int Genome::GetInputCount() const { return input_count_; }

int Genome::GetOutputCount() const { return output_count_; }

const std::vector<Neuron> &Genome::GetNeurons() const { return neurons_; }

const std::vector<Link> &Genome::GetLinks() const { return links_; }

void Genome::AddNeuron(NeuronType type, double bias) {
  int id = neurons_.size();
  neurons_.push_back(Neuron(next_neuron_id_, type, bias));
  ++next_neuron_id_;
}

void Genome::AddLink(int in_id, int out_id, double weight) {
  int id = links_.size();
  links_.push_back(Link(next_link_id_, in_id, out_id, weight));
  ++next_link_id_;
}

}  // namespace neat