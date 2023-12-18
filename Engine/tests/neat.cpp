#include <gtest/gtest.h>

#include "neat/neat-genome.h"
#include "neat/neat-link.h"
#include "neat/neat-neural-network.h"
#include "neat/neat-neuron.h"

/*!
 * @file neat.cpp
 * @brief Unit tests for NEAT Genome, Link, and Neuron functionalities.
 *
 * @details This file contains tests to validate the construction and operation
 * of the NEAT Genome, Link, and Neuron classes.
 */

using namespace neat;

/*!
 * @brief Tests the Genome constructor for correct initialization of input and
 * output counts.
 *
 * @details Ensures that a Genome object is correctly initialized with the
 * specified number of input and output neurons.
 */
TEST(NeatTests, GenomeConstructor) {
  int input_count = 5;
  int output_count = 3;
  Genome genome(input_count, output_count);

  EXPECT_EQ(genome.GetInputCount(), input_count);
  EXPECT_EQ(genome.GetOutputCount(), output_count);
}

/*!
 * @brief Tests adding a neuron to the Genome.
 *
 * @details Validates that a new neuron can be added to the Genome and that its
 * properties are correctly set.
 */
TEST(NeatTests, AddNeuron) {
  Genome genome(3, 2);
  genome.AddNeuron(Neuron(NeuronType::kHidden, 0.5));

  auto neurons = genome.GetNeurons();
  EXPECT_EQ(neurons.back().GetType(), NeuronType::kHidden);
  EXPECT_EQ(neurons.back().GetBias(), 0.5);
}

/*!
 * @brief Tests adding a link to the Genome.
 *
 * @details Ensures that a new link can be added to the Genome and that its
 * properties are correctly set.
 */
TEST(NeatTests, AddLink) {
  Genome genome(3, 2);
  genome.AddLink(Link(1, 2, 0.5));

  auto links = genome.GetLinks();
  EXPECT_EQ(links.back().GetInId(), 1);
  EXPECT_EQ(links.back().GetOutId(), 2);
  EXPECT_EQ(links.back().GetWeight(), 0.5);
}

/*!
 * @brief Tests the Link constructor for correct initialization.
 *
 * @details Validates that a Link object is correctly initialized with specified
 * input and output neuron IDs and weight.
 */
TEST(NeatTests, LinkConstructor) {
  int in_id = 2;
  int out_id = 3;
  double weight = 0.5;
  Link link(in_id, out_id, weight);

  EXPECT_EQ(link.GetInId(), in_id);
  EXPECT_EQ(link.GetOutId(), out_id);
  EXPECT_EQ(link.GetWeight(), weight);

  Link link2(in_id, out_id, weight);
  EXPECT_EQ(link.GetId(), link2.GetId() - 1);

  Link link3(in_id, out_id, weight);
  EXPECT_EQ(link2.GetId(), link3.GetId() - 1);
}

/*!
 * @brief Tests setting a new weight for a Link.
 *
 * @details Ensures that the weight of a Link can be changed and that the new
 * weight is correctly reflected.
 */
TEST(NeatTests, SetWeight) {
  Link link(2, 3, 0.5);
  double new_weight = 0.7;
  link.SetWeight(new_weight);

  EXPECT_EQ(link.GetWeight(), new_weight);
}

/*!
 * @brief Tests the Neuron constructor for correct initialization.
 *
 * @details Validates that a Neuron object is correctly initialized with a
 * specified type and bias.
 */
TEST(NeatTests, NeuronConstructor) {
  int id = 1;
  NeuronType type = NeuronType::kHidden;
  double bias = 0.5;
  Neuron neuron(type, bias);

  EXPECT_EQ(neuron.GetType(), type);
  EXPECT_EQ(neuron.GetBias(), bias);

  Neuron neuron2(type, bias);
  EXPECT_EQ(neuron.GetId(), neuron2.GetId() - 1);

  Neuron neuron3(type, bias);
  EXPECT_EQ(neuron2.GetId(), neuron3.GetId() - 1);
}

/*!
 * @brief Tests setting a new bias for a Neuron.
 *
 * @details Ensures that the bias of a Neuron can be changed and that the new
 * bias is correctly reflected.
 */
TEST(NeatTests, SetBias) {
  Neuron neuron(NeuronType::kHidden, 0.5);
  double new_bias = 0.7;
  neuron.SetBias(new_bias);

  EXPECT_EQ(neuron.GetBias(), new_bias);
}

// REENABLE WHEN DISABLE NEURON IS IMPLEMENTED
// TEST(NeatTests, DisableNeuron) {
//     Genome genome(5, 3);
//     genome.AddNeuron(Neuron(NeuronType::kHidden, 0.5));
//     genome.AddNeuron(Neuron(NeuronType::kHidden, 0.7));
//     genome.AddLink(Link(1, 2, 0.5));
//     genome.DisableNeuron(1);

//     auto neurons = genome.GetNeurons();
//     auto links = genome.GetLinks();

//     bool isNeuronDisabled = false;
//     for (const auto& neuron : neurons) {
//         if (neuron.GetId() == 1 && !neuron.IsActive()) {
//             isNeuronDisabled = true;
//             break;
//         }
//     }
//     ASSERT_TRUE(isNeuronDisabled);

//     for (const auto& link : links) {
//         EXPECT_TRUE(link.GetInId() != 1 || !link.IsActive());
//         EXPECT_TRUE(link.GetOutId() != 1 || !link.IsActive());
//     }
// }

/*!
 * @brief Tests disabling a link in the Genome.
 *
 * @details Ensures that a link can be disabled and that its active state is
 * correctly updated.
 */
TEST(NeatTests, DisableLink) {
  Genome genome(3, 2);
  genome.AddLink(Link(1, 2, 0.5));
  genome.AddLink(Link(2, 3, 0.7));
  int id = genome.GetLinks().back().GetId();
  genome.DisableLink(id);

  auto links = genome.GetLinks();

  bool isLinkDisabled = false;
  for (const auto& link : links) {
    if (link.GetId() == id) {
      ASSERT_TRUE(!link.IsActive());
    } else {
      ASSERT_TRUE(link.IsActive());
    }
  }
}

// REENABLE WHEN ENABLE NEURON IS IMPLEMENTED
// TEST(NeatTests, EnableNeuron) {
//     Genome genome(3, 2);
//     genome.AddNeuron(Neuron(NeuronType::kHidden, 0.5));
//     genome.AddNeuron(Neuron(NeuronType::kHidden, 0.4));
//     genome.AddNeuron(Neuron(NeuronType::kOutput, 0.3));
//     genome.DisableNeuron(1);
//     genome.EnableNeuron(1);

//     const auto& neurons = genome.GetNeurons();
//     bool anyEnabled = false;
//     for (const auto& neuron : neurons) {
//         if (neuron.IsActive()) {
//             anyEnabled = true;
//             break;
//         }
//     }

//     ASSERT_TRUE(anyEnabled);
// }

/*!
 * @brief Tests enabling a previously disabled link in the Genome.
 *
 * @details Ensures that a link can be re-enabled and that its active state is
 * correctly updated.
 */
TEST(NeatTests, EnableLink) {
  Genome genome(3, 2);
  genome.AddLink(Link(1, 2, 0.5));
  genome.AddLink(Link(2, 3, 0.7));
  int id = genome.GetLinks().back().GetId();
  genome.DisableLink(id);
  genome.EnableLink(id);

  const auto& links = genome.GetLinks();
  bool anyEnabled = false;
  for (const auto& link : links) {
    if (link.IsActive()) {
      anyEnabled = true;
      break;
    }
  }

  ASSERT_TRUE(anyEnabled);
}

/*!
 * @brief Tests removing a neuron from the Genome.
 *
 * @details Validates that a neuron and its associated links can be removed from
 * the Genome.
 */
TEST(NeatTests, RemoveNeuron) {
  Genome genome(3, 2);
  Neuron neuron1(NeuronType::kHidden, 0.5);
  Neuron neuron2(NeuronType::kOutput, 0.3);
  Neuron neuron3(NeuronType::kInput, 0.2);

  genome.AddNeuron(neuron1);
  genome.AddNeuron(neuron2);
  genome.AddNeuron(neuron3);
  genome.AddLink(Link(neuron1.GetId(), neuron2.GetId(), 0.7));
  genome.AddLink(Link(neuron2.GetId(), neuron3.GetId(), 0.4));
  int id = neuron1.GetId();
  genome.RemoveNeuron(id);

  const auto& neurons = genome.GetNeurons();
  const auto& links = genome.GetLinks();

  ASSERT_EQ(neurons.size(), 7);
  ASSERT_EQ(links.size(), 1);
  EXPECT_TRUE(links[0].GetInId() != 0 && links[0].GetOutId() != 0);
}

/*!
 * @brief Tests removing a link from the Genome.
 *
 * @details Ensures that a link can be removed from the Genome.
 */
TEST(NeatTests, RemoveLink) {
  Genome genome(3, 2);
  genome.AddLink(Link(1, 2, 0.5));
  genome.AddLink(Link(2, 3, 0.7));
  genome.RemoveLink(genome.GetLinks().back().GetId());

  ASSERT_EQ(genome.GetLinks().size(), 1);
}

/*!
 * @brief Tests various mutation operations on the Genome.
 *
 * @details Validates that the Genome can undergo various mutations, including
 * adding and removing neurons and links, and changing weights.
 */
TEST(NeatTests, Mutate) {
  auto HasAnyWeightChanged = [](const Genome& original, const Genome& mutated) {
    const std::vector<Link>& originalLinks = original.GetLinks();
    const std::vector<Link>& mutatedLinks = mutated.GetLinks();

    for (const Link& originalLink : originalLinks) {
      for (const Link& mutatedLink : mutatedLinks) {
        if (originalLink.GetId() == mutatedLink.GetId() &&
            originalLink.GetWeight() != mutatedLink.GetWeight()) {
          return true;
        }
      }
    }

    return false;
  };

  Genome genome(3, 3);

  int countAddNeuron = 0;
  int countAddLink = 0;
  int countRemoveNeuron = 0;
  int countRemoveLink = 0;
  int countChangeWeight = 0;

  for (int i = 0; i < 1000; i++) {
    Genome originalGenome = genome;
    genome.Mutate();

    if (genome.GetNeurons().size() > originalGenome.GetNeurons().size()) {
      countAddNeuron++;
    }
    if (genome.GetLinks().size() > originalGenome.GetLinks().size()) {
      countAddLink++;
    }
    if (genome.GetNeurons().size() < originalGenome.GetNeurons().size()) {
      countRemoveNeuron++;
    }
    if (genome.GetLinks().size() < originalGenome.GetLinks().size()) {
      countRemoveLink++;
    }
    if (HasAnyWeightChanged(genome, originalGenome)) {
      countChangeWeight++;
    }
  }

  // Check that each mutation happened at least once
  EXPECT_GT(countAddNeuron, 0);
  EXPECT_GT(countAddLink, 0);
  EXPECT_GT(countRemoveNeuron, 0);
  EXPECT_GT(countRemoveLink, 0);
  EXPECT_GT(countChangeWeight, 0);
}

/*!
 * @brief Tests removing a neuron via mutation in the Genome.
 *
 * @details Ensures that a neuron can be removed from the Genome via mutation.
 */
TEST(NeatTests, MutateRemoveNeuron) {
  Genome genome(3, 2);
  genome.AddNeuron(Neuron(NeuronType::kHidden, 0.5));
  genome.AddNeuron(Neuron(NeuronType::kHidden, 0.4));
  genome.AddNeuron(Neuron(NeuronType::kOutput, 0.3));
  genome.MutateRemoveNeuron();

  ASSERT_EQ(genome.GetNeurons().size(), 7);
}

/*!
 * @brief Tests removing a link via mutation in the Genome.
 *
 * @details Ensures that a link can be removed from the Genome via mutation.
 */
TEST(NeatTests, MutateRemoveLink) {
  Genome genome(3, 2);
  genome.AddLink(Link(1, 2, 0.5));
  genome.AddLink(Link(2, 3, 0.7));
  genome.MutateRemoveLink();

  ASSERT_EQ(genome.GetLinks().size(), 1);
}

/*!
 * @brief Tests changing the weights of links in the Genome via mutation.
 *
 * @details Validates that the weights of links in the Genome can be altered
 * through mutation.
 */
TEST(NeatTests, MutateChangeWeight) {
  Genome genome(105, 3);

  // Add 100 links with varying weights
  for (int i = 0; i < 100; i++) {
    double weight = 0.5 + static_cast<double>(i) /
                              200;  // weights will vary from 0.5 to 1.0
    genome.AddLink(Link(i, i + 1, weight));
  }

  // Store the original weights
  std::vector<double> original_weights;
  for (const auto& link : genome.GetLinks()) {
    original_weights.push_back(link.GetWeight());
  }

  // Mutate the weights
  genome.MutateChangeWeight();

  // Check if at least one weight has changed
  bool weight_changed = false;
  int i = 0;
  for (const auto& link : genome.GetLinks()) {
    if (original_weights[i] != link.GetWeight()) {
      weight_changed = true;
      break;
    }
    i++;
  }
  EXPECT_TRUE(weight_changed);

  // Store the second set of weights
  std::vector<double> new_weights;
  for (const auto& link : genome.GetLinks()) {
    new_weights.push_back(link.GetWeight());
  }

  // Mutate the weights again
  genome.MutateChangeWeight();

  // Check if at least one weight has changed
  weight_changed = false;
  i = 0;
  for (const auto& link : genome.GetLinks()) {
    if (new_weights[i] != link.GetWeight()) {
      weight_changed = true;
      break;
    }
    i++;
  }
  EXPECT_TRUE(weight_changed);
}

/*!
 * @brief Tests generating layers of neurons from a Genome.
 *
 * @details Validates that the get_layers function correctly organizes neurons
 * into layers based on their connections and types.
 */
TEST(NeatTests, GetLayers) {
  Genome genome(3, 2);
  genome.AddLink(Link(0, 3, 1));
  genome.AddLink(Link(2, 4, 1));
  genome.AddLink(Link(1, 4, 1));
  genome.AddLink(Link(2, 3, 1));
  genome.AddNeuron(Neuron(NeuronType::kHidden, 0.5));
  genome.AddLink(Link(0, 5, 1));
  genome.AddLink(Link(5, 3, 1));

  const std::vector<Neuron>& neurons = genome.GetNeurons();
  std::vector<std::vector<Neuron>> true_layers = {
      {neurons[0], neurons[1], neurons[2]},
      {neurons[5]},
      {neurons[3], neurons[4]}};
  std::vector<std::vector<Neuron>> result_layers = get_layers(genome);

  ASSERT_EQ(result_layers.size(), true_layers.size());
  for (size_t i = 0; i < result_layers.size(); i++) {
    ASSERT_EQ(result_layers[i].size(), true_layers[i].size());
    for (size_t j = 0; j < result_layers[i].size(); j++) {
      EXPECT_EQ(true_layers[i][j].GetId(), result_layers[i][j].GetId());
    }
  }
}

/*!
 * @brief Tests the activation function of the NeuralNetwork constructed from a
 * Genome.
 *
 * @details Ensures that the NeuralNetwork activates correctly with given input
 * values and produces expected output values.
 */
TEST(NeatTests, NeuralNetworkActivate) {
  Genome genome(3, 2);
  genome.AddLink(Link(0, 3, 1));
  genome.AddLink(Link(2, 4, 1));
  genome.AddLink(Link(1, 4, 1));
  genome.AddLink(Link(2, 3, 1));
  genome.AddNeuron(Neuron(NeuronType::kHidden, 0.5));
  genome.AddLink(Link(0, 5, 1));
  genome.AddLink(Link(5, 3, 1));
  NeuralNetwork neural_network(genome);
  std::vector<double> input_values = {1, 1, 1};
  std::vector<double> output_values = neural_network.Activate(input_values);

  ASSERT_FALSE(
      output_values.empty());  // Replace with more specific checks as needed
}

/*!
 * @brief Tests the crossover functionality for Neuron objects.
 *
 * @details Validates that two neurons can be combined to create a new neuron
 * with properties derived from both parent neurons.
 */
TEST(NeatTests, CrossoverNeuron) {
  Neuron neuronA{NeuronType::kHidden, 0.3};
  Neuron neuronB{NeuronType::kHidden, 0.7};
  EXPECT_ANY_THROW(CrossoverNeuron(neuronA, neuronB));

  neuronB = neuronA;
  neuronB.SetBias(0.7);

  int countA = 0;
  int countB = 0;
  for (int i = 0; i < 1000; i++) {
    Neuron crossoverResult = CrossoverNeuron(neuronA, neuronB);
    EXPECT_EQ(crossoverResult.GetId(), neuronA.GetId());
    EXPECT_EQ(crossoverResult.GetType(), NeuronType::kHidden);
    EXPECT_EQ(crossoverResult.IsActive(), neuronA.IsActive());
    EXPECT_GE(crossoverResult.GetBias(), 0.0);
    EXPECT_LE(crossoverResult.GetBias(), 1.0);

    if (crossoverResult.GetBias() == neuronA.GetBias()) {
      countA++;
    } else if (crossoverResult.GetBias() == neuronB.GetBias()) {
      countB++;
    }
  }

  // Check that both biases were used in the crossover
  EXPECT_GT(countA, 0);
  EXPECT_GT(countB, 0);
}

/*!
 * @brief Tests the crossover functionality for Link objects.
 *
 * @details Ensures that two links can be combined to create a new link with
 * properties derived from both parent links.
 */
TEST(NeatTests, CrossoverLink) {
  Link linkA{10, 20, 0.3};
  Link linkB{10, 20, 0.7};

  EXPECT_ANY_THROW(CrossoverLink(linkA, linkB));

  linkB = linkA;
  linkB.SetWeight(0.7);

  int countA = 0;
  int countB = 0;
  for (int i = 0; i < 1000; i++) {
    Link crossoverResult = CrossoverLink(linkA, linkB);
    EXPECT_EQ(crossoverResult.GetId(), linkA.GetId());
    EXPECT_EQ(crossoverResult.GetInId(), 10);
    EXPECT_EQ(crossoverResult.GetOutId(), 20);
    EXPECT_EQ(crossoverResult.IsActive(), linkA.IsActive());
    EXPECT_GE(crossoverResult.GetWeight(), 0.0);
    EXPECT_LE(crossoverResult.GetWeight(), 1.0);

    if (crossoverResult.GetWeight() == linkA.GetWeight()) {
      countA++;
    } else if (crossoverResult.GetWeight() == linkB.GetWeight()) {
      countB++;
    }
  }

  // Check that both weights were used in the crossover
  EXPECT_GT(countA, 0);
  EXPECT_GT(countB, 0);
}

/*!
 * @brief Tests the crossover functionality between two Genomes.
 *
 * @details Validates that two Genomes can be combined to create a new Genome
 * with characteristics inherited from both parents.
 */
TEST(NeatTests, Crossover) {
  Genome genomeA(3, 3);

  // Mutate genomeA several times
  for (int i = 0; i < 100; i++) {
    genomeA.Mutate();
  }

  // Clone genomeA to create genomeB
  Genome genomeB = genomeA;

  // Mutate both genomes several times
  for (int i = 0; i < 100; i++) {
    genomeA.Mutate();
    genomeB.Mutate();
  }

  // Perform crossover
  Genome crossoverResult = Crossover(genomeA, genomeB);

  // Check that every neuron ID in crossoverResult also exists in genomeA and
  // vice versa
  for (const auto& neuron : crossoverResult.GetNeurons()) {
    EXPECT_NE(
        std::find_if(
            genomeA.GetNeurons().begin(), genomeA.GetNeurons().end(),
            [&neuron](const Neuron& n) { return n.GetId() == neuron.GetId(); }),
        genomeA.GetNeurons().end());
  }

  for (const auto& neuron : genomeA.GetNeurons()) {
    EXPECT_NE(std::find_if(crossoverResult.GetNeurons().begin(),
                           crossoverResult.GetNeurons().end(),
                           [&neuron](const Neuron& n) {
                             return n.GetId() == neuron.GetId();
                           }),
              crossoverResult.GetNeurons().end());
  }

  // Check that every link ID in crossoverResult also exists in genomeA and vice
  // versa
  for (const auto& link : crossoverResult.GetLinks()) {
    EXPECT_NE(std::find_if(
                  genomeA.GetLinks().begin(), genomeA.GetLinks().end(),
                  [&link](const Link& l) { return l.GetId() == link.GetId(); }),
              genomeA.GetLinks().end());
  }

  for (const auto& link : genomeA.GetLinks()) {
    EXPECT_NE(std::find_if(
                  crossoverResult.GetLinks().begin(),
                  crossoverResult.GetLinks().end(),
                  [&link](const Link& l) { return l.GetId() == link.GetId(); }),
              crossoverResult.GetLinks().end());
  }

  // Check that the bias of every neuron in crossoverResult corresponds to the
  // bias of the same neuron in genomeA or genomeB
  for (const auto& neuron : crossoverResult.GetNeurons()) {
    auto itA = std::find_if(
        genomeA.GetNeurons().begin(), genomeA.GetNeurons().end(),
        [&neuron](const Neuron& n) { return n.GetId() == neuron.GetId(); });
    auto itB = std::find_if(
        genomeB.GetNeurons().begin(), genomeB.GetNeurons().end(),
        [&neuron](const Neuron& n) { return n.GetId() == neuron.GetId(); });

    if (itA != genomeA.GetNeurons().end() &&
        neuron.GetBias() == itA->GetBias()) {
      // Neuron bias matches with genomeA
    } else if (itB != genomeB.GetNeurons().end() &&
               neuron.GetBias() == itB->GetBias()) {
      // Neuron bias matches with genomeB
    } else {
      FAIL() << "Neuron with ID " << neuron.GetId()
             << " not found in either parent genome or biases do not match.";
    }
  }

  // Check that the weight of every link in crossoverResult corresponds to the
  // weight of the same link in genomeA or genomeB
  for (const auto& link : crossoverResult.GetLinks()) {
    auto itA = std::find_if(
        genomeA.GetLinks().begin(), genomeA.GetLinks().end(),
        [&link](const Link& l) { return l.GetId() == link.GetId(); });
    auto itB = std::find_if(
        genomeB.GetLinks().begin(), genomeB.GetLinks().end(),
        [&link](const Link& l) { return l.GetId() == link.GetId(); });

    if (itA != genomeA.GetLinks().end() &&
        link.GetWeight() == itA->GetWeight()) {
      // Link weight matches with genomeA
    } else if (itB != genomeB.GetLinks().end() &&
               link.GetWeight() == itB->GetWeight()) {
      // Link weight matches with genomeB
    } else {
      FAIL() << "Link with ID " << link.GetId()
             << " not found in either parent genome or weights do not match.";
    }
  }
}

/*!
 * @brief Tests the functionality of NeuralNetwork activation after mutations
 * and crossover of Genomes.
 *
 * @details Ensures that a NeuralNetwork constructed from a Genome that
 * underwent mutations and crossover activates correctly.
 */
TEST(NeatTests, ActivationAfterMutateCrossover) {
  Genome genomeA(3, 1);

  // Mutate genomeA several times
  for (int i = 0; i < 1000; i++) {
    genomeA.Mutate();
    // for (auto &link : genomeA.GetLinks()) {
    //     std::cerr << link.GetInId() << ' ' << link.GetOutId() << std::endl;
    // }
    // std::cerr << std::endl;
  }

  // Clone genomeA to create genomeB
  Genome genomeB = genomeA;

  // Mutate both genomes several times
  for (int i = 0; i < 1000; i++) {
    genomeA.Mutate();
    genomeB.Mutate();
  }

  // Perform crossover
  Genome crossoverResult = Crossover(genomeA, genomeB);

  NeuralNetwork nn = NeuralNetwork(crossoverResult);
  std::cerr << nn.Activate({1, 1, 1})[0] << std::endl;
  std::cerr << nn.Activate({1, 0, 1})[0] << std::endl;
  std::cerr << nn.Activate({0, 1, 1})[0] << std::endl;
}
