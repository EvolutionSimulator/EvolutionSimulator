#ifndef NEATNEURALNETWORK_H
#define NEATNEURALNETWORK_H

#endif // NEATNEURALNETWORK_H

#include "neat/neat-genome.h"

namespace neat {

class NeuronInput {
public:
    NeuronInput(int input_id, double weight);
    int input_id;
    double weight;
};

class FeedForwardNeuron { // Functional units of the NeuralNetowork, obtained from Neurons and Links
public:
    FeedForwardNeuron(double value_, double bias_, std::vector<NeuronInput> inputs_);
    double value;
    double bias;
    std::vector<NeuronInput> inputs;

};

class NeuralNetwork {
public:
    NeuralNetwork(Genome &genom);
private:
    std::vector<int> input_ids_;
    std::vector<int> output_ids_;
    std::vector<FeedForwardNeuron> neurons_;
};

std::vector<std::vector<int> > get_layers(Genome genom); //organize neurons from a Genome into layers (by neurons ids)

bool contains(std::vector<int> v, int el);

} //end of namespace neat
