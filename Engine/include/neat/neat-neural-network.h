#ifndef NEATNEURALNETWORK_H
#define NEATNEURALNETWORK_H

#endif // NEATNEURALNETWORK_H

#include "neat/neat-genome.h"

namespace neat {

struct NeuronInput {
    NeuronInput(int input_id, double weight);
    int input_id;
    double weight;
};

struct FeedForwardNeuron { // Functional units of the NeuralNetowork, obtained from Neurons and Links
    FeedForwardNeuron(int id, double bias_, std::vector<NeuronInput> inputs_);
    int id;
    double bias;
    std::vector<NeuronInput> inputs;

};

class NeuralNetwork {
public:
    NeuralNetwork(Genome &genom);
    std::vector<double> Activate(std::vector<double> input_values) const;
private:
    std::vector<int> input_ids_;
    std::vector<int> output_ids_;
    std::vector<FeedForwardNeuron> ffneurons_;
};

std::vector<std::vector<Neuron> > get_layers(Genome &genom); //organize neurons from a Genome into layers (by neurons ids)

template <typename T> bool contains(std::vector<T> v, T el){ //check if a vector contains an element
    for(typename std::vector<T>::iterator i = v.begin(); i != v.end(); i ++) {
        if ((*i) == el) {
            return true;
        }
    }
    return false;
}

double activation_funtion(double x);

} //end of namespace neat
