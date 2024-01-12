#ifndef BRAINMODULE_H
#define BRAINMODULE_H

#include <vector>

class BrainModule
{
public:
    BrainModule(int input, int output, int module_id);

    int GetFirstInputIndex();
    void SetFirstInputIndex(const int index);

    int GetFirstOutputIndex();
    void SetFirstOutputIndex(const int index);

    std::vector<int> GetInputNeuronIds();
    void SetInputNeuronIds(const std::vector<int> input_neuron_ids);

    std::vector<int> GetOutputNeuronIds();
    void SetOutputNeuronIds(const std::vector<int> input_neuron_ids);

    int GetModuleId();

private:
    int first_input_index_;
    int first_output_index_;
    std::vector<int> input_neuron_ids_;
    std::vector<int> output_neuron_ids_;

    int module_id_; /*! Unique identifier for a type of module */
};



#endif // BRAINMODULE_H
