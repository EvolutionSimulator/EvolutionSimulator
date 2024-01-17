#include "neat/brain_module.h"

#include <random>

BrainModule::BrainModule(int input, int output, int module_id, bool multiple)
    : input_neuron_ids_(input, 0), output_neuron_ids_(output, 0),
      module_id_(module_id), multiple_(multiple) {
    // Create a random number generator
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, 15); // Uniform distribution between 1 and 16

    // Use the distribution to generate a random number
    type_ = dis(gen);
}

int BrainModule::GetFirstInputIndex() {
    return first_input_index_;
}

void BrainModule::SetFirstInputIndex(const int index) {
    first_input_index_ = index;
}

int BrainModule::GetFirstOutputIndex() {
    return first_output_index_;
}

void BrainModule::SetFirstOutputIndex(const int index) {
    first_output_index_ = index;
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

bool BrainModule::GetMultiple() {
    return multiple_;
}

int BrainModule::GetType() {
    return type_;
}
