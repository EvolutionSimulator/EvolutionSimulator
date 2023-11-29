#include "neat/neat-neuron.h"

namespace neat {

Neuron::Neuron(int id, NeuronType type, double bias)
    : id_(id), type_(type), bias_(bias) {}

int Neuron::GetId() const { return id_; }

NeuronType Neuron::GetType() const { return type_; }

double Neuron::GetBias() const { return bias_; }

bool Neuron::IsActive() const { return active_; }

void Neuron::SetBias(double bias) { bias_ = bias; }

void Neuron::SetActive(){active_= false;}

void Neuron::SetInactive(){active_ = true;}

}  // namespace neat
