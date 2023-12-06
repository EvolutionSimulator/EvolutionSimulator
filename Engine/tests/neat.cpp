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
