#include "neat/neat-neuron.h"
/*!
 * @file neat-neuron.h
 *
 * @brief Defines the Neuron class and related functions for NEAT.
 */

#include "random.h"
#include "stdexcept"

namespace neat {

int Neuron::next_id_ = 1;

/*!
 * @brief Constructs a Neuron with specified type and bias.
 *
 * @param type The type of the neuron (input, output, hidden).
 * @param bias The bias of the neuron.
 */
Neuron::Neuron(NeuronType type, double bias)
    : id_(next_id_++), type_(type), bias_(bias), active_(true),
      activation_(ActivationType::linear){}

/*!
 * @brief Gets the unique identifier of the neuron.
 *
 * @return The ID of the neuron.
 */
int Neuron::GetId() const { return id_; }

/*!
 * @brief Gets the type of the neuron.
 *
 * @return The type of the neuron (input, output, hidden).
 */
NeuronType Neuron::GetType() const { return type_; }

/*!
 * @brief Gets the activation function of the neuron
 *
 * @return The activation function of the neuron.
 */
ActivationType Neuron::GetActivation() const { return activation_; }


/*!
 * @brief Gets the bias of the neuron.
 *
 * @return The bias of the neuron.
 */
double Neuron::GetBias() const { return bias_; }

/*!
 * @brief Checks if the neuron is active.
 *
 * @return True if the neuron is active, false otherwise.
 */
bool Neuron::IsActive() const { return active_; }

/*!
 * @brief Sets the bias of the neuron.
 *
 * @param bias The new bias to be set for the neuron.
 */
void Neuron::SetBias(double bias) { bias_ = bias; }

/*!
 * @brief Sets the neuron to be active.
 */
void Neuron::SetActive() { active_ = true; }

/*!
 * @brief Sets the neuron to be inactive.
 */
void Neuron::SetInactive() { active_ = false; }

/*!
 * @brief Sets the activation type of the neuron.
 */
void Neuron::SetActivation(ActivationType activation){  activation_ = activation; }


/*!
 * @brief Performs crossover between two Neurons to produce a new Neuron.
 *
 * @param a The first Neuron in the crossover.
 * @param b The second Neuron in the crossover.
 *
 * @return A new Neuron resulting from the crossover of the two neurons.
 *
 * @throws std::invalid_argument if the two neurons have different IDs or types.
 *
 * @details The crossover Neuron's bias is randomly chosen from either 'a' or
 * 'b'.
 */
Neuron CrossoverNeuron(const Neuron &a, const Neuron &b) {
  if (a.GetId() != b.GetId()) {
    throw std::invalid_argument("Neurons must have the same id");
  }

  if (a.GetType() != b.GetType()) {
    throw std::invalid_argument("Neurons must have the same type");
  }

  int id = a.GetId();
  NeuronType type = a.GetType();

  Neuron crossover_neuron = a;

  crossover_neuron.SetBias(Random::Double(0.0, 1.0) < 0.5 ? a.GetBias() : b.GetBias());

  return crossover_neuron;
}
}  // namespace neat
