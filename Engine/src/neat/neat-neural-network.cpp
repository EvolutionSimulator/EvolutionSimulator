#include "neat/neat-neural-network.h"
//#include "neat/neat-neuron.h"
#include "neat/neat-genome.h"

/*!
 * @file neat-neural-network.h
 *
 * @brief Defines the NeuralNetwork class and related functions for NEAT.
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <unordered_map>

namespace neat {

/*!
 * @brief  Constructs a NeuralNetwork object
 from a genome object, assuming the genome
 represents a valid neural network (ie the cyclic
 links are labeled as such etc.)
 *
 * @param genome The Genome to construct the NeuralNetwork from.
 */
NeuralNetwork::NeuralNetwork(const Genome &genom){
  std::vector<std::vector<Neuron> > layers = get_layers(genom);
  std::vector<FeedForwardNeuron> ffneurons;

  for (const Neuron &neuron : layers.front()) {
    input_ids_.push_back(neuron.GetId());
  }
  for (const Neuron &neuron : layers.back()) {
    output_ids_.push_back(neuron.GetId());
  }

  for (const std::vector<Neuron> &layer : layers) {
    for (const Neuron &neuron : layer) {
      std::vector<NeuronInput> inputs;
      std::vector<NeuronInput> inputs_from_cycles;
      for (const Link &link : genom.GetLinks()) {
        if (link.GetOutId() == neuron.GetId()) {
          NeuronInput input = {link.GetInId(), link.GetWeight()};
          if (link.IsCyclic()) {
            inputs_from_cycles.push_back(input);
          } else {
            inputs.push_back(input);
          }
        }
      }
      FeedForwardNeuron ffneuron = {
          neuron.GetId(), neuron.GetBias(),   0.0,
          inputs,         inputs_from_cycles, neuron.GetActivation()};

      ffneurons.push_back(ffneuron);
    }
  }
  ffneurons_ = ffneurons;
}

std::vector<FeedForwardNeuron> NeuralNetwork::GetNeurons() const {
  return ffneurons_;
}

/*!
 * @brief Produces an output of the
                                neural network based on the values of input
                             neurons (input_values vector) and the values of
                             neurons involved in cycles (stored inside each
                             ffneuron). Also updates the values of neurons
                             involved in cycles.
 *
 * @param input_values The input values to feed into the network.
 *
 * @return A vector of output values after network activation.
 */
std::vector<double> NeuralNetwork::Activate(
    const std::vector<double> &input_values) {
  assert(input_values.size() == input_ids_.size());
  std::unordered_map<int, double> values;
  for (int i = 0; i < input_values.size(); i++) {
    values[input_ids_[i]] = input_values[i];
  }

  for (const FeedForwardNeuron &ffneuron : ffneurons_) {
    if (values.find(ffneuron.id) ==
        values.end()) {  // if ffneuron is not already activated
      double value = ffneuron.value;
      for (const NeuronInput &input : ffneuron.inputs) {
        // assert(values.find(input.input_id) != values.end()); //previous
        // neurons have to be activated
        if (values.find(input.input_id) != values.end()) {
          value += values[input.input_id] * input.weight;
        }
      }
      value += ffneuron.bias;

      if (std::find(output_ids_.begin(), output_ids_.end(), ffneuron.id) ==
          output_ids_.end()) {
        value=activation_function(ffneuron.activation, value);
      }

      values[ffneuron.id] = value;
    }
  }

  // update values of ffneurons. this affects those neurons which start a cycle
  for (FeedForwardNeuron &ffneuron : ffneurons_) {
    ffneuron.value = 0;
    for (const NeuronInput &neuron_input : ffneuron.inputs_from_cycles) {
      ffneuron.value += neuron_input.weight * values[neuron_input.input_id];
    }
  }
  std::vector<double> result;
  for (int i = 0; i < output_ids_.size(); i++) {
    result.push_back(values[output_ids_[i]]);
  }
  return result;
}

/*!
 * @brief Constructs layers of neurons from a given Genome.
 *
 * @param genome The Genome to construct the layers from.
 *
 * @return A vector of neuron layers, where each layer is a vector of Neuron.
 */
std::vector<std::vector<Neuron> > get_layers(const Genome &genom) {
  std::vector<std::vector<Neuron> > layers;
  std::vector<Neuron> input_layer;
  std::vector<Neuron> output_layer;
  std::vector<int>
      active;  // a neuron is active if all of its incoming neurons belong to an
               // active layer. A layer is active if all of its neurons are
               // active.
               // input neurons are also active and they form an active layer
  std::vector<Neuron> neurons = genom.GetNeurons();
  std::vector<Link> links = genom.GetLinks();

  for (const Neuron &neuron : neurons) {
    if (neuron.GetType() == NeuronType::kInput) {
      int inputid = neuron.GetId();
      input_layer.push_back(neuron);
      active.push_back(inputid);
    }
  }
  layers.push_back(input_layer);
  for (const Neuron &neuron : neurons) {
    if (neuron.GetType() == NeuronType::kOutput) {
      output_layer.push_back(neuron);
    }
  }
  int N_neurons = neurons.size();  // total number of neurons in the network
  while (active.size() < N_neurons - genom.GetOutputCount()) {
    std::vector<Neuron> layer;
    for (const Neuron &neuron : neurons) {
      if (neuron.GetType() == NeuronType::kHidden &&
          std::find(active.begin(), active.end(), neuron.GetId()) ==
              active.end()) {
        bool is_active = true;
        for (const Link &link : links) {
          if (link.IsActive() && !link.IsCyclic() &&
              link.GetOutId() == neuron.GetId() &&
              std::find(active.begin(), active.end(), link.GetInId()) ==
                  active.end()) {
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
    for (std::vector<Neuron>::iterator i = layer.begin(); i != layer.end();
         i++) {
      active.push_back((*i).GetId());
    }
  }
  layers.push_back(output_layer);
  return layers;
}

/*!
 * @brief Activation function used in the neural network.
 *
 * @param n the neuron to be activated, x The input value to the activation function.
 *
 * @return The output of the activation function.
 */
double activation_function(ActivationType n, double x) {
    switch (n) {
        case ActivationType::sigmoid:
            return 1/(1+exp(-x));
        case ActivationType::tanh:
            return (exp(x)-exp(-x))/(exp(x)+exp(-x));
        case ActivationType::relu:
            return std::max(0.0,x);
        case ActivationType::leakyRelu:
            return std::max(0.1*x, x);
        case ActivationType::binary:
            return (x >= 0.0) ? 1.0 : 0.0;
        case ActivationType::linear:
            return x;
        default:
            return x;
    }
}
//double activation_function(double x) { return 1 / (1 + exp(-x)); }
}  // end of namespace neat
