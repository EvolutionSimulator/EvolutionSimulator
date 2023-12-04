#include <cassert>
#include <iostream>

#include "neat/neat-genome.h"
#include "neat/neat-link.h"
#include "neat/neat-neuron.h"

void TestGenomeConstructor() {
  int input_count = 5;
  int output_count = 3;
  neat::Genome genome(input_count, output_count);

  assert(genome.GetInputCount() == input_count);
  assert(genome.GetOutputCount() == output_count);
  std::cout << "Genome constructor test passed!\n";
}

void TestAddNeuron() {
  neat::Genome genome(3, 2);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.5);

  auto neurons = genome.GetNeurons();
  assert(neurons.back().GetType() == neat::NeuronType::kHidden);
  assert(neurons.back().GetBias() == 0.5);
  std::cout << "AddNeuron test passed!\n";
}

void TestAddLink() {
  neat::Genome genome(3, 2);
  genome.AddLink(1, 2, 0.5);

  auto links = genome.GetLinks();
  assert(links.back().GetInId() == 1);
  assert(links.back().GetOutId() == 2);
  assert(links.back().GetWeight() == 0.5);
  std::cout << "AddLink test passed!\n";
}

void TestLinkConstructor() {
  int id = 1;
  int in_id = 2;
  int out_id = 3;
  double weight = 0.5;
  neat::Link link(id, in_id, out_id, weight);

  assert(link.GetId() == id);
  assert(link.GetInId() == in_id);
  assert(link.GetOutId() == out_id);
  assert(link.GetWeight() == weight);
  std::cout << "Link constructor test passed!\n";
}

void TestSetWeight() {
  neat::Link link(1, 2, 3, 0.5);
  double new_weight = 0.7;
  link.SetWeight(new_weight);

  assert(link.GetWeight() == new_weight);
  std::cout << "SetWeight test passed!\n";
}

void TestNeuronConstructor() {
  int id = 1;
  neat::NeuronType type = neat::NeuronType::kHidden;
  double bias = 0.5;
  neat::Neuron neuron(id, type, bias);

  assert(neuron.GetId() == id);
  assert(neuron.GetType() == type);
  assert(neuron.GetBias() == bias);
  std::cout << "Neuron constructor test passed!\n";
}

void TestSetBias() {
  neat::Neuron neuron(1, neat::NeuronType::kHidden, 0.5);
  double new_bias = 0.7;
  neuron.SetBias(new_bias);

  assert(neuron.GetBias() == new_bias);
  std::cout << "SetBias test passed!\n";
}

void TestMutateChangeWeight() {
  neat::Genome genome(5, 3);

  // Add some links
  genome.AddLink(0, 1, 0.5);
  genome.AddLink(1, 2, 0.3);
  genome.AddLink(2, 3, 0.7);

  // Store the original weights
  std::vector<double> original_weights;
  for (const auto& link : genome.GetLinks()) {
    original_weights.push_back(link.GetWeight());
  }

  // Mutate the weights
  genome.MutateChangeWeight();

  // Check if the weights have changed
  int i = 0;
  for (const auto& link : genome.GetLinks()) {
    assert(original_weights[i] != link.GetWeight());
    i++;
  }
  std::cout << "MutateChangeWeight test passed!\n";
}

void TestAllNeat() {
  TestGenomeConstructor();
  TestAddNeuron();
  TestAddLink();
  TestLinkConstructor();
  TestSetWeight();
  TestNeuronConstructor();
  TestSetBias();
  TestMutateChangeWeight();
}