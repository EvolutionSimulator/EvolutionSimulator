#include "neat/neat-neuron.h"

#include "random"
#include "stdexcept"
namespace neat {

int Neuron::next_id_ = 1;

Neuron::Neuron(NeuronType type, double bias)
    : id_(next_id_++), type_(type), bias_(bias), active_(true) {}

int Neuron::GetId() const { return id_; }

NeuronType Neuron::GetType() const { return type_; }

double Neuron::GetBias() const { return bias_; }

bool Neuron::IsActive() const { return active_; }

void Neuron::SetBias(double bias) { bias_ = bias; }

void Neuron::SetActive() { active_ = true; }

void Neuron::SetInactive() { active_ = false; }

Neuron CrossoverNeuron(const Neuron &a, const Neuron &b) {
  if (a.GetId() != b.GetId()) {
    throw std::invalid_argument("Neurons must have the same id");
  }

  if (a.GetType() != b.GetType()) {
    throw std::invalid_argument("Neurons must have the same type");
  }

  int id = a.GetId();
  NeuronType type = a.GetType();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  Neuron crossover_neuron = a;

  crossover_neuron.SetBias(distribution(gen) < 0.5 ? a.GetBias() : b.GetBias());

  return crossover_neuron;
}
}  // namespace neat
