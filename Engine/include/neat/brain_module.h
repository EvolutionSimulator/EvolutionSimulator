#ifndef BRAINMODULE_H
#define BRAINMODULE_H

#include <vector>

class BrainModule
{
public:
    BrainModule(int input, int output);

    int GetFirstInputId();
    void SetFirstInputId(const int id);

    int GetFirstOutputId();
    void SetFirstOutputId(const int id);

    std::vector<int> GetInputNeuronIds();
    void SetInputNeuronIds(const std::vector<int> input_neuron_ids);

    std::vector<int> GetOutputNeuronIds();
    void SetOutputNeuronIds(const std::vector<int> input_neuron_ids);

private:
    int first_input_id_;
    int first_output_id_;
    std::vector<int> input_neuron_ids_;
    std::vector<int> output_neuron_ids_;
};

#endif // BRAINMODULE_H
