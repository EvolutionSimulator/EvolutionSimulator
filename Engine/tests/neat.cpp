#include <gtest/gtest.h>
#include "neat/neat-genome.h"
#include "neat/neat-link.h"
#include "neat/neat-neuron.h"

TEST(NeatTests, GenomeConstructor) {
    int input_count = 5;
    int output_count = 3;
    neat::Genome genome(input_count, output_count);

    EXPECT_EQ(genome.GetInputCount(), input_count);
    EXPECT_EQ(genome.GetOutputCount(), output_count);
}

TEST(NeatTests, AddNeuron) {
    neat::Genome genome(3, 2);
    genome.AddNeuron(neat::NeuronType::kHidden, 0.5);

    auto neurons = genome.GetNeurons();
    EXPECT_EQ(neurons.back().GetType(), neat::NeuronType::kHidden);
    EXPECT_EQ(neurons.back().GetBias(), 0.5);
}

TEST(NeatTests, AddLink) {
    neat::Genome genome(3, 2);
    genome.AddLink(1, 2, 0.5);

    auto links = genome.GetLinks();
    EXPECT_EQ(links.back().GetInId(), 1);
    EXPECT_EQ(links.back().GetOutId(), 2);
    EXPECT_EQ(links.back().GetWeight(), 0.5);
}

TEST(NeatTests, LinkConstructor) {
    int id = 1;
    int in_id = 2;
    int out_id = 3;
    double weight = 0.5;
    neat::Link link(id, in_id, out_id, weight);

    EXPECT_EQ(link.GetId(), id);
    EXPECT_EQ(link.GetInId(), in_id);
    EXPECT_EQ(link.GetOutId(), out_id);
    EXPECT_EQ(link.GetWeight(), weight);
}

TEST(NeatTests, SetWeight) {
    neat::Link link(1, 2, 3, 0.5);
    double new_weight = 0.7;
    link.SetWeight(new_weight);

    EXPECT_EQ(link.GetWeight(), new_weight);
}

TEST(NeatTests, NeuronConstructor) {
    int id = 1;
    neat::NeuronType type = neat::NeuronType::kHidden;
    double bias = 0.5;
    neat::Neuron neuron(id, type, bias);

    EXPECT_EQ(neuron.GetId(), id);
    EXPECT_EQ(neuron.GetType(), type);
    EXPECT_EQ(neuron.GetBias(), bias);
}

TEST(NeatTests, SetBias) {
    neat::Neuron neuron(1, neat::NeuronType::kHidden, 0.5);
    double new_bias = 0.7;
    neuron.SetBias(new_bias);

    EXPECT_EQ(neuron.GetBias(), new_bias);
}
TEST(NeatTests, CrossoverNeuron) {
    neat::Neuron neuronA{1, neat::NeuronType(), 0.3};
    neat::Neuron neuronB{1, neat::NeuronType(), 0.7};
    neat::Neuron crossoverResult = CrossoverNeuron(neuronA, neuronB);
    EXPECT_EQ(crossoverResult.GetId(), 1);
    EXPECT_EQ(crossoverResult.GetType(), neat::NeuronType());
    EXPECT_GE(crossoverResult.GetBias(), 0.0);
    EXPECT_LE(crossoverResult.GetBias(), 1.0);
    std::cout << "CrossoverNeuron test passed." << std::endl;
}
TEST(NeatTests, CrossoverLink) {
    neat::Link linkA{1, 10, 20, 0.3};
    neat::Link linkB{1, 10, 20, 0.7};
    neat::Link crossoverResult = CrossoverLink(linkA, linkB);

    EXPECT_EQ(crossoverResult.GetId(), 1);
    EXPECT_EQ(crossoverResult.GetInId(), 10);
    EXPECT_EQ(crossoverResult.GetOutId(), 20);
    EXPECT_GE(crossoverResult.GetWeight(), 0.0);
    EXPECT_LE(crossoverResult.GetWeight(), 1.0);

    std::cout << "CrossoverLink test passed." << std::endl;
}
TEST(NeatTests, CrossoverGenome) {
    neat::Genome dominantGenome{2, 1};
    neat::Genome recessiveGenome{2, 1};
    dominantGenome.AddNeuron(neat::NeuronType(), 0.3);
    recessiveGenome.AddNeuron( neat::NeuronType(), -0.5);
    dominantGenome.AddLink(1, 2, 0.5);
    recessiveGenome.AddLink(1, 2, 0.7);
    neat::Genome offspringGenome = Crossover(dominantGenome, recessiveGenome);
    EXPECT_EQ(offspringGenome.GetInputCount(), 2);
    EXPECT_EQ(offspringGenome.GetOutputCount(), 1);
    const auto& offspringNeurons = offspringGenome.GetNeurons();
    EXPECT_EQ(offspringNeurons[0].GetType(), neat::NeuronType());
    EXPECT_GE(offspringNeurons[0].GetBias(), -1.0);
    EXPECT_LE(offspringNeurons[0].GetBias(), 1.0);
    const auto& offspringLinks = offspringGenome.GetLinks();
    EXPECT_EQ(offspringLinks.size(), 1);
    EXPECT_EQ(offspringLinks[0].GetInId(), 1);
    EXPECT_EQ(offspringLinks[0].GetOutId(), 2);
    EXPECT_GE(offspringLinks[0].GetWeight(), 0.0);
    EXPECT_LE(offspringLinks[0].GetWeight(), 1.0);
    std::cout << "Crossover test passed." << std::endl;
}
