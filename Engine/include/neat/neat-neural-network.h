#ifndef NEATNEURALNETWORK_H
#define NEATNEURALNETWORK_H

#endif  // NEATNEURALNETWORK_H

#include "neat/neat-genome.h"

namespace neat {

/*!
 * @struct NeuronInput
 *
 * @brief Represents an input connection to a neuron.
 *
 * @details This structure holds the information about an input connection to a
 * neuron, including the ID of the input neuron and the weight of the
 * connection.
 */
struct NeuronInput {
  int input_id;  /*!< The ID of the input neuron. */
  double weight; /*!< The weight of the connection from the input neuron. */
};

/*!
 * @struct FeedForwardNeuron
 *
 * @brief Represents a neuron in a feed-forward neural network.
 *
 * @details This structure is used to represent a neuron in the neural network
 * constructed from a NEAT genome. It includes the neuron's ID, its bias, and
 * its input connections.
 */
struct FeedForwardNeuron {  // Functional units of the NeuralNetowork, obtained
                            // from Neurons and Links
  int id;                   /*!< The ID of the neuron. */
  double bias;              /*!< The bias of the neuron. */
  std::vector<NeuronInput> inputs; /*!< A vector of NeuronInput representing the
                                      inputs to this neuron. */
};

/*!
 * @class NeuralNetwork
 *
 * @brief Represents a neural network constructed from a NEAT genome.
 *
 * @details This class translates a Genome into a feed-forward neural network
 * structure.
 */
class NeuralNetwork {
 public:
  NeuralNetwork(const Genome &genom);
  std::vector<double> Activate(const std::vector<double> &input_values) const;

 private:
  std::vector<int> input_ids_;  /*!< IDs of input neurons. */
  std::vector<int> output_ids_; /*!< IDs of output neurons. */
  std::vector<FeedForwardNeuron>
      ffneurons_; /*!< Feed-forward neurons used in the network. */
};

std::vector<std::vector<Neuron> > get_layers(
    const Genome
        &genom);  // organize neurons from a Genome into layers (by neurons ids)

double activation_function(double x);

}  // end of namespace neat
