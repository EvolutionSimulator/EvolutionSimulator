#include "neat/neat-neural-network.h"

namespace neat {

NeuronInput::NeuronInput(int input_id, double weight){
    this->input_id = input_id;
    this->weight = weight;
}

FeedForwardNeuron::FeedForwardNeuron(double value, double bias, std::vector<NeuronInput> inputs) {
    this->value = value;
    this->bias = bias;
    this->inputs = inputs;
}

NeuralNetwork::NeuralNetwork(Genome &genom) {
    /*
    std::vector< std::vector<int> > layers = get_layers(genom);
    std::vector<Neuron> neurons = genom.GetNeurons();
    std::vector<FeedForwardNeuron> ffneurons;
    for (int i=0; i < genom.GetInputCount(); i++) {
        input_ids_.push_back(neurons[i].GetId());
    }
    for (int i=genom.GetInputCount(); i < genom.GetInputCount() + genom.GetOutputCount(); i++) {
        output_ids_.push_back(neurons[i].GetId());
    }
    */
}

std::vector<std::vector<int> > get_layers(Genome &genom) {
    std::vector<std::vector<int> > layers;
    std::vector<int> input_layer;
    std::vector<int> active; // a neuron is active if all of its incoming neurons belong to an active layer. A layer is active if all of its neurons are active.
                             //input neurons are also active and they form an active layer
    std::vector<Neuron> neurons = genom.GetNeurons();
    std::vector<Link> links = genom.GetLinks();

    for (int i = 0; i < genom.GetInputCount(); i++) {
        int inputid = neurons[i].GetId();
        input_layer.push_back(inputid);
        active.push_back(inputid);
    }
    layers.push_back(input_layer);
    /*std::vector<int> ouput_layer;
    for (int i = genom.GetInputCount(); i < genom.GetInputCount() + genom.GetOutputCount(); i++) {
        int outputid = genom.GetNeurons()[i].GetId();
        output_layer.push_back(outputid);
    }*/
    int N_neurons = genom.GetNeurons().size(); //total number of neurons in the network
    while (active.size() < N_neurons) {
        std::vector<int> layer;
        for (int i = genom.GetInputCount(); i < N_neurons;  i++){ //we dont check for the input neurons, they are already added
            Neuron neuron = neurons[i];
            bool is_active = true;
            for (const Link& link: links) {
                if (link.IsActive() && link.GetInId() == neuron.GetId() && contains(active, link.GetOutId())) {
                    is_active = false;
                    break;
                }
            }
            if (is_active) {
                layer.push_back(neuron.GetId());
            }
        }
        layers.push_back(layer);
        active.reserve(active.size() + distance(layer.begin(), layer.end()));
        active.insert(active.end(), layer.begin(), layer.end());
    }
    return layers;
}

bool contains(std::vector<int> v, int el) {
    for(std::vector<int>::iterator i = v.begin(); i != v.end(); i ++) {
        if ((*i) == el) {
            return true;
        }
    }
    return false;
}

} // end of namespace neat
