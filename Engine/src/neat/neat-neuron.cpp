#include "neat/neat-neuron.h"
#include "assert.h"
#include "random"
namespace neat {

Neuron::Neuron(int id, NeuronType type, double bias)
    : id_(id), type_(type), bias_(bias) {}

int Neuron::GetId() const { return id_; }

NeuronType Neuron::GetType() const { return type_; }

double Neuron::GetBias() const { return bias_; }

bool Neuron::IsActive() const { return active_; }

void Neuron::SetBias(double bias) { bias_ = bias; }

void Neuron::SetActive(){active_= true;}

void Neuron::SetInactive(){active_ = false;}

Neuron CrossoverNeuron(const Neuron &a, const Neuron &b){
    assert(a.GetId() == b.GetId());
    assert(a.GetType() == b.GetType());
    int id = a.GetId();
    NeuronType type = a.GetType();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double bias = distribution(gen) < 0.5 ? a.GetBias() : b.GetBias();

    return {id, type, bias};
}
}  // namespace neat
