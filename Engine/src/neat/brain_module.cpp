#include "neat/brain_module.h"

BrainModule::BrainModule(int input, int output)
    : input_neuron_ids_(input, 0), output_neuron_ids_(output, 0) { }
