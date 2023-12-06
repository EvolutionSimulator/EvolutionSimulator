#ifndef NEATNEURON_H
#define NEATNEURON_H

#include <vector>

namespace neat {

enum class NeuronType { kInput, kOutput, kHidden };

class Neuron {
 public:
  Neuron(int id, NeuronType type, double bias);

  int GetId() const;
  NeuronType GetType() const;
  double GetBias() const;
  bool IsActive() const;

  void SetBias(double bias);
  void SetActive();
  void SetInactive();
 private:
  int id_;
  bool active_ = true;
  NeuronType type_;
  double bias_;
};

Neuron CrossoverNeuron(const Neuron &a, const Neuron &b);

}  // namespace neat

#endif  // NEATNEURON_H
