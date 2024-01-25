#ifndef BRAINMODULE_H
#define BRAINMODULE_H

#include <vector>

class BrainModule
{
public:
    BrainModule(int input, int output, int module_id, bool multiple = false);

    int GetFirstInputIndex() const;
    void SetFirstInputIndex(const int index);

    int GetFirstOutputIndex() const;
    void SetFirstOutputIndex(const int index);

    std::vector<int> GetInputNeuronIds() const;
    void SetInputNeuronIds(const std::vector<int> input_neuron_ids);

    std::vector<int> GetOutputNeuronIds() const;
    void SetOutputNeuronIds(const std::vector<int> input_neuron_ids);

    int GetModuleId() const;

    bool GetMultiple() const;

    int GetType() const;

private:
    int first_input_index_;
    int first_output_index_;
    std::vector<int> input_neuron_ids_;
    std::vector<int> output_neuron_ids_;

    int module_id_; /*! Unique identifier for a type of module */
    bool multiple_; /*! Boolean that indicates if multiple instances of
                      this module can be evolved by the same creature */
    int type_; /*! Random int that identifies the type of module if there
                 can be multiplicity (i.e. type of pheromone) */
};



#endif // BRAINMODULE_H
