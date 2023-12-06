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

void TestEnableNeuron() {
  neat::Genome genome(3, 2);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.5);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.4);
  genome.AddNeuron(neat::NeuronType::kOutput, 0.3);

  // Disable a neuron
  genome.DisableNeuron(1); // Assuming ID 1 for the neuron to disable

  genome.EnableNeuron(1); // Enable the previously disabled neuron

  const auto& neurons = genome.GetNeurons();
  bool anyEnabled = false;
  for (const auto& neuron : neurons) {
      if (neuron.IsActive()) {
          anyEnabled = true;
          break;
      }
  }

  assert(anyEnabled); // At least one neuron should have been enabled
  std::cout << "EnableNeuron test passed!\n";
}

void TestEnableLink() {
  neat::Genome genome(3, 2);
  genome.AddLink(1, 2, 0.5);
  genome.AddLink(2, 3, 0.7);

  // Disable a link
  genome.DisableLink(0); // Assuming ID 0 for the link to disable

  genome.EnableLink(0); // Enable the previously disabled link

  const auto& links = genome.GetLinks();
  bool anyEnabled = false;
  for (const auto& link : links) {
      if (link.IsActive()) {
          anyEnabled = true;
          break;
      }
  }

  assert(anyEnabled); // At least one link should have been enabled
  std::cout << "EnableLink test passed!\n";
}

void TestRemoveNeuron() {
  // Create a genome with a few neurons and links
  neat::Genome genome(3, 2);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.5);  // ID 0
  genome.AddNeuron(neat::NeuronType::kOutput, 0.3);  // ID 1
  genome.AddNeuron(neat::NeuronType::kInput, 0.2);   // ID 2
  genome.AddLink(0, 1, 0.7); // Create a link connected to the neuron to be removed
  genome.AddLink(1, 2, 0.4); // Create another link not associated with the neuron to be removed

  genome.RemoveNeuron(0); // Removing neuron with ID 0

  const auto& neurons = genome.GetNeurons();
  const auto& links = genome.GetLinks();

  assert(neurons.size() == 7);
  assert(neurons[0].GetId() == 1);
  assert(neurons[1].GetId() == 2);

  assert(links.size() == 1);
  assert(links[0].GetInId() != 0 && links[0].GetOutId() != 0);

  std::cout << "RemoveNeuron test passed!\n";
}


void TestRemoveLink() {
  neat::Genome genome(3, 2);
  genome.AddLink(1, 2, 0.5);
  genome.AddLink(2, 3, 0.7);

  // Assuming ID 0 for the link to remove
  genome.RemoveLink(0);

  assert(genome.GetLinks().size() == 1);
  std::cout << "RemoveLink test passed!\n";
}

void TestMutateRemoveNeuron() {
  neat::Genome genome(3, 2);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.5);
  genome.AddNeuron(neat::NeuronType::kHidden, 0.4);
  genome.AddNeuron(neat::NeuronType::kOutput, 0.3);

  genome.MutateRemoveNeuron();

  assert(genome.GetNeurons().size() == 7);
  std::cout << "MutateRemoveNeuron test passed!\n";
}

void TestMutateRemoveLink() {
  neat::Genome genome(3, 2);
  genome.AddLink(1, 2, 0.5);
  genome.AddLink(2, 3, 0.7);

  genome.MutateRemoveLink();

  assert(genome.GetLinks().size() == 1);
  std::cout << "MutateRemoveLink test passed!\n";
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

  // Store the second set of weights
  std::vector<double> new_weights;
  for (const auto& link : genome.GetLinks()) {
    new_weights.push_back(link.GetWeight());
  }

  // Mutate the weights again
  genome.MutateChangeWeight();

  // Check if the weights have changed
  i = 0;
  for (const auto& link : genome.GetLinks()) {
    assert(new_weights[i] != link.GetWeight());
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
  TestDisableNeuron();
  TestDisableLink();
  TestEnableNeuron();
  TestEnableLink();
  TestRemoveNeuron();
  TestRemoveLink();
  TestMutateRemoveNeuron();
  TestMutateRemoveLink();
  TestMutateChangeWeight();
}
