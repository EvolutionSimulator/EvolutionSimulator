#include "neat/brain_module.h"

BrainModule::BrainModule(int input, int output, int module_id)
    : input_neuron_ids_(input, 0), output_neuron_ids_(output, 0),
      module_id_(module_id) { }

int BrainModule::GetFirstInputId() {
    return first_input_id_;
}

void BrainModule::SetFirstInputId(const int id) {
    first_input_id_ = id;
}

int BrainModule::GetFirstOutputId() {
    return first_output_id_;
}

void BrainModule::SetFirstOutputId(const int id) {
    first_output_id_ = id;
}

std::vector<int> BrainModule::GetInputNeuronIds() {
    return input_neuron_ids_;
}

void BrainModule::SetInputNeuronIds(const std::vector<int> input_neuron_ids) {
    input_neuron_ids_ = input_neuron_ids;
}

std::vector<int> BrainModule::GetOutputNeuronIds() {
    return output_neuron_ids_;
}

void BrainModule::SetOutputNeuronIds(const std::vector<int> output_neuron_ids) {
    output_neuron_ids_ = output_neuron_ids;
}

int BrainModule::GetModuleId() {
    return module_id_;
}
