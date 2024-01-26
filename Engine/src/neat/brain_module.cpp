#include "neat/brain_module.h"

#include "random.h"

BrainModule::BrainModule(int input, int output, int module_id, bool multiple)
    : input_neuron_ids_(input, 0), output_neuron_ids_(output, 0),
      module_id_(module_id), multiple_(multiple),
      first_input_index_(0), first_output_index_(0){
    type_ = Random::Int(0, 15);
}

int BrainModule::GetFirstInputIndex() const {
    return first_input_index_;
}

void BrainModule::SetFirstInputIndex(const int index) {
    first_input_index_ = index;
}

int BrainModule::GetFirstOutputIndex() const {
    return first_output_index_;
}

void BrainModule::SetFirstOutputIndex(const int index) {
    first_output_index_ = index;
}

std::vector<int> BrainModule::GetInputNeuronIds() const {
    return input_neuron_ids_;
}

void BrainModule::SetInputNeuronIds(const std::vector<int> input_neuron_ids) {
    input_neuron_ids_ = input_neuron_ids;
}

std::vector<int> BrainModule::GetOutputNeuronIds() const {
    return output_neuron_ids_;
}

void BrainModule::SetOutputNeuronIds(const std::vector<int> output_neuron_ids) {
    output_neuron_ids_ = output_neuron_ids;
}

int BrainModule::GetModuleId() const {
    return module_id_;
}

bool BrainModule::GetMultiple() const {
    return multiple_;
}

int BrainModule::GetType() const {
    return type_;
}
