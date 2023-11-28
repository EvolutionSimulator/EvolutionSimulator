#ifndef NEATNEURON_H
#define NEATNEURON_H

namespace neat {

enum class NeuronType { kInput, kOutput, kHidden };

class Neuron {
 public:
  Neuron(int id, NeuronType type, double bias);

  int GetId() const;
  NeuronType GetType() const;
  double GetBias() const;

  void SetBias(double bias);

 private:
  int id_;
  NeuronType type_;
  double bias_;
};

}  // namespace neat

#endif  // NEATNEURON_H