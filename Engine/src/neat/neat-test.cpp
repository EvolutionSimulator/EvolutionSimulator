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

void TestDisableNeuron() {
  // Create a test genome with neurons and links
  neat::Genome genome(5, 3);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.5);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.7);
  genome.AddLink(1, 2, 0.5);

  // Disable a neuron by its ID
  genome.DisableNeuron(1);

  auto neurons = genome.GetNeurons();
  auto links = genome.GetLinks();

  // Check if the neuron with ID 1 is disabled
  bool isNeuronDisabled = false;
  for (const auto& neuron : neurons) {
      if (neuron.GetId() == 1 && !neuron.IsActive()) {
          isNeuronDisabled = true;
          break;
      }
  }
  assert(isNeuronDisabled);

  // Check if all links related to the disabled neuron are also disabled
  for (const auto& link : links) {
      assert(link.GetInId() != 1 || !link.IsActive());
      assert(link.GetOutId() != 1 || !link.IsActive());
  }

  std::cout << "DisableNeuron test passed!\n";
}

void TestDisableLink() {
  // Create a test genome with links
  neat::Genome genome(3, 2);
  genome.AddLink(1, 2, 0.5);
  genome.AddLink(2, 3, 0.7);

  // Disable a link by its ID
  genome.DisableLink(1);

  auto links = genome.GetLinks();

  // Check if the link with ID 1 is disabled
  bool isLinkDisabled = false;
  for (const auto& link : links) {
      if (link.GetId() == 1 && !link.IsActive()) {
          isLinkDisabled = true;
          break;
      }
  }
  assert(isLinkDisabled);

  std::cout << "DisableLink test passed!\n";
}

void TestMutateDisableNeuron() {
  neat::Genome genome(4, 2);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.5);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.7);

  // Ensure that at least one hidden neuron exists
  assert(genome.GetNeurons().size() >= 1);

  // Disable a random hidden neuron
  genome.MutateDisableNeuron();

  auto neurons = genome.GetNeurons();
  auto links = genome.GetLinks();

  // Check if the disabled neuron is indeed disabled
  bool isNeuronDisabled = false;
  for (const auto& neuron : neurons) {
      if (neuron.GetType() == neat::NeuronType::kHidden && !neuron.IsActive()) {
          isNeuronDisabled = true;
          break;
      }
  }
  assert(isNeuronDisabled);

  // Check if all links related to the disabled neuron are also disabled
  for (const auto& link : links) {
      assert(link.GetInId() != 1 || !link.IsActive());
      assert(link.GetOutId() != 1 || !link.IsActive());
  }

  std::cout << "MutateDisableNeuron test passed!\n";
}

void TestMutateDisableLink() {
  neat::Genome genome(3, 3);
  genome.AddLink(1, 2, 0.5);
  genome.AddLink(2, 3, 0.7);

  // Ensure that at least one link exists
  assert(genome.GetLinks().size() >= 1);

  // Disable a random link
  genome.MutateDisableLink();

  auto links = genome.GetLinks();

  // Check if the disabled link is indeed disabled
  bool isLinkDisabled = false;
  for (const auto& link : links) {
      if (!link.IsActive()) {
          isLinkDisabled = true;
          break;
      }
  }
  assert(isLinkDisabled);

  std::cout << "MutateDisableLink test passed!\n";
}

void TestAllNeat() {
  TestGenomeConstructor();
  TestAddNeuron();
  TestAddLink();
  TestLinkConstructor();
  TestSetWeight();
  TestNeuronConstructor();
  TestSetBias();
  /*
  TestDisableNeuron();
  TestDisableLink();
  TestMutateDisableNeuron();
  TestMutateDisableLink();
  */
}
