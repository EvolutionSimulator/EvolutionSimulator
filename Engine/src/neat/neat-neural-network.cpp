#include "neat/neat-neural-network.h"
#include <unordered_map>
#include <cmath>
#include <cassert>

namespace neat {

NeuronInput::NeuronInput(int input_id, double weight){
    this->input_id = input_id;
    this->weight = weight;
}

FeedForwardNeuron::FeedForwardNeuron(int id, double bias, std::vector<NeuronInput> inputs) {
    this->id = id;
    this->bias = bias;
    this->inputs = inputs;
}

NeuralNetwork::NeuralNetwork(Genome &genom) {
    std::vector< std::vector<Neuron> > layers = get_layers(genom);
    //std::vector<Neuron> neurons = genom.GetNeurons();
    std::vector<FeedForwardNeuron> ffneurons;
    std::vector<int> input_ids;
    for (const Neuron &neuron:layers.front()){
        input_ids.push_back(neuron.GetId());
    }
    input_ids_ = input_ids;
    std::vector<int> output_ids;
    for (const Neuron &neuron:layers.back()){
        output_ids.push_back(neuron.GetId());
    }
    output_ids_ = output_ids;
    for (const std::vector<Neuron> &layer: layers) {
        for (const Neuron &neuron : layer) {
            std::vector<NeuronInput> inputs;
            for (const Link &link : genom.GetLinks()) {
                if (link.GetOutId() == neuron.GetId()) {
                    inputs.push_back(NeuronInput(link.GetInId(), link.GetWeight()));
                }
            }
            ffneurons.push_back(FeedForwardNeuron(neuron.GetId(), neuron.GetBias(), inputs));
        }
    }
    ffneurons_ = ffneurons;
}

std::vector<double> NeuralNetwork::Activate(std::vector<double> input_values) const {
    assert(input_values.size() == input_ids_.size());
    std::unordered_map<int, double> values;
    for (int i = 0; i < input_values.size(); i++) {
        values[input_ids_[i]] = input_values[i];
    }
    /*for (int i = 0; i < output_ids_.size(); i++) {
        values[output_ids_[i]] = 0;
    }*/
    for (const FeedForwardNeuron &ffneuron:ffneurons_) {
        if (values.find(ffneuron.id) == values.end()) {//if ffneuron is not already activated
            double value = 0;
            for (const NeuronInput &input:ffneuron.inputs) {
                assert(values.find(input.input_id) != values.end()); //previous neurons have to be activated
                value += values[input.input_id] * input.weight;
            }
            value+= ffneuron.bias;
            value = activation_funtion(value);
            values[ffneuron.id] = value;
        }
    }
    std::vector<double> result;
    for (int i  = 0; i < output_ids_.size(); i++) {
        result.push_back(values[output_ids_[i]]);
    }
    return result;

}

std::vector<std::vector<Neuron> > get_layers(Genome &genom) {
    std::vector<std::vector<Neuron> > layers;
    std::vector<Neuron> input_layer;
    std::vector<int> active; // a neuron is active if all of its incoming neurons belong to an active layer. A layer is active if all of its neurons are active.
                             //input neurons are also active and they form an active layer
    std::vector<Neuron> neurons = genom.GetNeurons();
    std::vector<Link> links = genom.GetLinks();

    for (int i = 0; i < genom.GetInputCount(); i++) {
        int inputid = neurons[i].GetId();
        input_layer.push_back(neurons[i]);
        active.push_back(inputid);
    }
    layers.push_back(input_layer);
    std::vector<Neuron> output_layer;
    for (int i = genom.GetInputCount(); i < genom.GetInputCount() + genom.GetOutputCount(); i++) {
        output_layer.push_back(neurons[i]);
    }
    int N_neurons = neurons.size(); //total number of neurons in the network
    while (active.size() < N_neurons - genom.GetOutputCount()) {
        std::vector<Neuron> layer;
        for (int i = genom.GetInputCount() + genom.GetOutputCount(); i < N_neurons;  i++){ //we dont check for the input neurons, they are already added
            Neuron neuron = neurons[i];
            if (!contains(active, neuron.GetId())) {
                bool is_active = true;
                for (const Link& link: links) {
                    if (link.IsActive() && link.GetOutId() == neuron.GetId() && !contains(active, link.GetInId())) {
                        is_active = false;
                        break;
                    }
                }
                if (is_active) {
                    layer.push_back(neuron);
                }
            }
        }
        layers.push_back(layer);
        for (std::vector<Neuron>::iterator i = layer.begin(); i != layer.end(); i++) {
            active.push_back((*i).GetId());
        }

    }
    layers.push_back(output_layer);
    return layers;
}

double activation_funtion(double x){
    return 1 / (1 + exp(-x));
}
} // end of namespace neat
