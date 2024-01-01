#ifndef NEATNEURON_H
#define NEATNEURON_H

#include <vector>

namespace neat {

enum class NeuronType { kInput, kOutput, kHidden };
enum class ActivationType { sigmoid, relu, elu, leakyRelu, binary, linear };

/*!
 * @class Neuron
 *
 * @brief Represents a neuron in a NEAT neural network.
 *
 * @details Each neuron has a unique identifier, a type (input, output, hidden),
 * a bias, and an active state.
 */
class Neuron {
 public:
  explicit Neuron(NeuronType type, double bias);

  int GetId() const;
  NeuronType GetType() const;
  double GetBias() const;
  bool IsActive() const;

  void SetBias(double bias);
  void SetActive();
  void SetInactive();

  ActivationType GetActivation() const;
  void SetActivation(ActivationType a);

 private:
  static int next_id_; /*!< Static variable used to assign unique IDs to new
                          neurons. */
  int id_;             /*!< Unique identifier for the neuron. */
  NeuronType type_;    /*!< Type of the neuron (input, output, hidden). */
  double bias_;        /*!< Bias of the neuron. */
  bool active_;        /*!< Indicates whether the neuron is active or not. */
  ActivationType activation_=ActivationType::sigmoid;
};

Neuron CrossoverNeuron(const Neuron &a, const Neuron &b);

Neuron NewNeuron(NeuronType type, double bias);

}  // namespace neat

#endif  // NEATNEURON_H
