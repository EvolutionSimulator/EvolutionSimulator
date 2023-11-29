#include "neat/neat-genome.h"

namespace neat {

Genome::Genome(int input_count, int output_count)
    : input_count_(input_count),
      output_count_(output_count),
      next_neuron_id_(0),
      next_link_id_(0),
      neurons_(),
      links_() {
  for (int i = 0; i < input_count_; ++i) {
    AddNeuron(NeuronType::kInput, 0.0);
  }
  for (int i = 0; i < output_count_; ++i) {
    AddNeuron(NeuronType::kOutput, 0.0);
  }
}

int Genome::GetInputCount() const { return input_count_; }

int Genome::GetOutputCount() const { return output_count_; }

const std::vector<Neuron> &Genome::GetNeurons() const { return neurons_; }

const std::vector<Link> &Genome::GetLinks() const { return links_; }

void Genome::AddNeuron(NeuronType type, double bias) {
  int id = neurons_.size();
  neurons_.push_back(Neuron(next_neuron_id_, type, bias));
  ++next_neuron_id_;
}

void Genome::AddLink(int in_id, int out_id, double weight) {
  int id = links_.size();
  links_.push_back(Link(next_link_id_, in_id, out_id, weight));
  ++next_link_id_;
}

void Genome::DisableNeuron(int id) {
  for (Neuron& neuron: neurons_) {
    if (neuron.GetId()==id){
        neuron.SetInactive();
        // should we include this
        for (const Link& link: links_){
            if (neuron.GetId()==link.GetInId() || neuron.GetId()==link.GetOutId()){
                DisableLink(link.GetId());
            }
        }
        break;
    }
  }
}

void Genome::DisableLink(int id) {
  for (Link& link: links_){
    if (link.GetId()==id){
        link.SetInactive();
    }
  }
}


void Genome::EnableNeuron(int id) {
  for (Neuron& neuron: neurons_) {
    if (neuron.GetId()==id){
        neuron.SetActive();
        //kind of necessary if we disable all links when a neuron is disabled (but which ones should be reactivated)
        /*
        for (const Link& link: GetLinks()){
            if (neuron.GetId()==link.GetInId() || neuron.GetId()==link.GetOutId()){
                ActivateLink(link.GetId());
            }
        }
        */
        break;
    }
  }
}

void Genome::EnableLink(int id) {
  for (Link& link: links_){
    if (link.GetId()==id){
        link.SetActive();
    }
  }
}


void Genome::MutateDisableNeuron() {
  std::vector<int> hiddenIDs = {};
  for (const Neuron& neuron: GetNeurons()){
    if (neuron.GetType() == NeuronType::kHidden){
        hiddenIDs.push_back(neuron.GetId());
    }
  }
  if (!hiddenIDs.empty()) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, hiddenIDs.size() - 1);

    DisableNeuron(hiddenIDs[dis(gen)]);

    // do we want to implement so that some neurons can also be activated

  }
}

void Genome::MutateDisableLink() {
  if (!links_.empty()) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, links_.size() - 1);

    DisableLink(links_[dis(gen)].GetId());
  }

}

}  // namespace neat
