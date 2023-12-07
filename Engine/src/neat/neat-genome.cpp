#include "neat/neat-genome.h"

#include <algorithm>
#include <random>

namespace neat {

Genome::Genome(int input_count, int output_count) : neurons_(), links_() {
  for (int i = 0; i < input_count; ++i) {
    AddNeuron(Neuron(NeuronType::kInput, 0.0));
  }
  for (int i = 0; i < output_count; ++i) {
    AddNeuron(Neuron(NeuronType::kOutput, 0.0));
  }
}

int Genome::GetInputCount() const {
  return std::count_if(neurons_.begin(), neurons_.end(),
                       [](const Neuron& neuron) {
                         return neuron.GetType() == NeuronType::kInput;
                       });
}

int Genome::GetOutputCount() const {
  return std::count_if(neurons_.begin(), neurons_.end(),
                       [](const Neuron& neuron) {
                         return neuron.GetType() == NeuronType::kOutput;
                       });
}

const std::vector<Neuron>& Genome::GetNeurons() const { return neurons_; }

const std::vector<Link>& Genome::GetLinks() const { return links_; }

void Genome::AddNeuron(const Neuron& neuron) { neurons_.push_back(neuron); }

void Genome::AddLink(const Link& link) { links_.push_back(link); }

// The function should not be used for now
/*
void Genome::DisableNeuron(int id) {
  for (Neuron& neuron: neurons_) {
    if (neuron.GetId()==id){
        neuron.SetInactive();
        // should we include this
        for (const Link& link: links_){
            if (neuron.GetId()==link.GetInId() ||
neuron.GetId()==link.GetOutId()){ DisableLink(link.GetId());
            }
        }
        break;
    }
  }
}
*/

void Genome::DisableLink(int id) {
  for (Link& link : links_) {
    if (link.GetId() == id) {
      link.SetInactive();
    }
  }
}

// The function should not be used for now
/*
void Genome::EnableNeuron(int id) {
  for (Neuron& neuron: neurons_) {
    if (neuron.GetId()==id){
        neuron.SetActive();
        //kind of necessary if we disable all links when a neuron is disabled
(but which ones should be reactivated)
        // for (const Link& link: GetLinks()){
        //     if (neuron.GetId()==link.GetInId() ||
neuron.GetId()==link.GetOutId()){
        //         ActivateLink(link.GetId());
        //     }
        // }
        break;
    }
  }
}
*/

void Genome::EnableLink(int id) {
  for (Link& link : links_) {
    if (link.GetId() == id) {
      link.SetActive();
    }
  }
}

void Genome::RemoveNeuron(int id) {
  int index_to_delete = -1;
  for (int i = 0; i < neurons_.size(); ++i) {
    if (neurons_[i].GetId() == id) {
      index_to_delete = i;
      break;
    }
  }

  if (index_to_delete != -1) {
    std::vector<int> links_to_remove;

    for (const Link& link : links_) {
      if (neurons_[index_to_delete].GetId() == link.GetInId() ||
          neurons_[index_to_delete].GetId() == link.GetOutId()) {
        links_to_remove.push_back(link.GetId());
      }
    }

    for (int link_id : links_to_remove) {
      RemoveLink(link_id);
    }

    neurons_.erase(neurons_.begin() + index_to_delete);
  }
}

// void Genome::RemoveLink(int id) {
//   int index_to_delete = -1;
//   for (int i = 0; i<links_.size(); i++){
//     if (links_[i].GetId()==id){
//         index_to_delete = i;
//         break;
//     }
//   }
//   if (index_to_delete!=-1){
//     links_.erase(links_.begin()+index_to_delete);
//   }
// }

void Genome::Mutate() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  if (uniform(gen) < addNeuronMutationRate) {
    MutateAddNeuron();
  }

  if (uniform(gen) < addLinkMutationRate) {
    MutateAddLink();
  }

  if (uniform(gen) < removeNeuronMutationRate) {
    MutateRemoveNeuron();
  }

  if (uniform(gen) < removeLinkMutationRate) {
    MutateRemoveLink();
  }

  if (uniform(gen) < changeWeightMutationRate) {
    MutateChangeWeight();
  }
}

void Genome::MutateRemoveNeuron() {
  std::vector<int> hiddenIDs = {};
  for (const Neuron& neuron : GetNeurons()) {
    if (neuron.GetType() == NeuronType::kHidden) {
      hiddenIDs.push_back(neuron.GetId());
    }
  }
  if (!hiddenIDs.empty()) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, hiddenIDs.size() - 1);

    RemoveNeuron(hiddenIDs[dis(gen)]);
  }
}

void Genome::MutateRemoveLink() {
  if (!links_.empty()) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, links_.size() - 1);

    int index = dis(gen);
    int idToRemove = links_[index].GetId();

    RemoveLink(idToRemove);
  }
}

bool Genome::HasLink(const int& inID, const int& outID) {
  for (const auto& link : links_) {
    if (link.GetInId() == inID && link.GetOutId() == outID ||
        link.GetInId() == outID && link.GetOutId() == inID) {
      return true;
    }
  }
  return false;
}

void Genome::RemoveLink(int id) {
  int index_to_delete = -1;
  for (int i = 0; i < links_.size(); i++) {
    if (links_[i].GetId() == id) {
      index_to_delete = i;
      break;
    }
  }
  if (index_to_delete != -1) {
    links_.erase(links_.begin() + index_to_delete);
  }
}

void Genome::MutateChangeWeight() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);
  std::normal_distribution<> dis(0.0, standardDeviationWeight);

  for (Link& link : links_) {
    if (uniform(gen) < weightMutationRate) {
      double delta = dis(gen);
      link.SetWeight(link.GetWeight() + delta);

      if (link.GetWeight() > maxWeight) {
        link.SetWeight(maxWeight);
      } else if (link.GetWeight() < minWeight) {
        link.SetWeight(minWeight);
      }
    }
  }
}

// Neuron Genome::FindNeuronIndexById(int targetId) const {
//     for (size_t i = 0; i < neurons_.size(); ++i) {
//         if (neurons_[i].GetId() == targetId) {
//             // Neuron with the specified ID found, return its index
//             return neurons_[i];
//         }
//     }

//    // Neuron with the specified ID not found, return -1
//    return -1;
//}
bool Genome::DFS(const Neuron& currentNeuron, std::unordered_set<int>& visited,
                 std::unordered_set<int>& visiting) const {
  int currentId = currentNeuron.GetId();

  if (visiting.find(currentId) != visiting.end()) {
    return true;  // loop
  }

  if (visited.find(currentId) != visited.end()) {
    return false;  // already visited
  }

  visiting.insert(currentId);

  for (const auto& link : links_) {
    if (link.GetInId() == currentId) {
      int neighborId = link.GetOutId();
      auto neighborIt = std::find_if(
          neurons_.begin(), neurons_.end(),
          [neighborId](const Neuron& n) {  // find the neuron w right id
            return n.GetId() == neighborId;
          });

      if (neighborIt != neurons_.end() && DFS(*neighborIt, visited, visiting)) {
        return true;  // found loop
      }
    }
  }

  visiting.erase(currentId);
  visited.insert(currentId);

  return false;
}

bool Genome::DetectLoops(const Neuron& startNeuron) {
  std::unordered_set<int> visited;
  std::unordered_set<int> visiting;

  return DFS(startNeuron, visited, visiting);
}

void Genome::MutateAddLink() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, neurons_.size() - 1);
  size_t indexRandomNeuron1 = dist(gen);
  size_t indexRandomNeuron2 = dist(gen);
  while (neurons_[indexRandomNeuron1].GetType() == NeuronType::kOutput) {
    indexRandomNeuron1 = dist(gen);
  }
  while (neurons_[indexRandomNeuron2].GetType() == NeuronType::kInput) {
    indexRandomNeuron2 = dist(gen);
  }

  int n1 = neurons_[indexRandomNeuron1].GetId();  // id of in neuron
  int n2 = neurons_[indexRandomNeuron2].GetId();  // id of out neuron

  if (HasLink(n1, n2)) {
    return;  // IF WE END UP USING ENABLED/DISABLE LINKS, THEN ENABLE LINK
  }

  // Check if cycle exists:
  AddLink(Link(n1, n2, 1));
  Link newl = links_.back();
  if (DetectLoops(neurons_[indexRandomNeuron1])) {
    RemoveLink(newl.GetId());
    return;
  }
}

void Genome::MutateAddNeuron() {
  if (links_.size() == 0) {
    return;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, links_.size() - 1);
  size_t randIndex = dist(gen);
  Link RandomLink = links_[randIndex];
  DisableLink(RandomLink.GetId());  // test

  AddNeuron(Neuron(NeuronType::kHidden, 0.0));
  // disable the initial link between the inId and outId
  int newNeuronId = neurons_.back().GetId();
  AddLink(Link(RandomLink.GetInId(), newNeuronId, 1));
  AddLink(Link(newNeuronId, RandomLink.GetOutId(), RandomLink.GetWeight()));
}

Genome Crossover(const Genome& dominant, const Genome& recessive) {
  Genome offspring{0, 0};
  for (const auto& dominant_neuron : dominant.GetNeurons()) {
    int neuron_id = dominant_neuron.GetId();
    std::optional<Neuron> recessive_neuron;
    for (const auto& recessive_neuron_candidate : recessive.GetNeurons()) {
      if (recessive_neuron_candidate.GetId() == neuron_id) {
        recessive_neuron = recessive_neuron_candidate;
        break;
      }
    }
    if (!recessive_neuron) {
      offspring.AddNeuron(dominant_neuron);
    } else {
      offspring.AddNeuron(
          CrossoverNeuron(dominant_neuron, recessive_neuron.value()));
    }
  }
  for (const auto& dominant_link : dominant.GetLinks()) {
    int link_id = dominant_link.GetId();
    std::optional<Link> recessive_link;
    for (const auto& recessive_link_candidate : recessive.GetLinks()) {
      if (recessive_link_candidate.GetId() == link_id) {
        recessive_link = recessive_link_candidate;
        break;
      }
    }
    if (!recessive_link) {
      offspring.AddLink(dominant_link);
    } else {
      offspring.AddLink(CrossoverLink(dominant_link, recessive_link.value()));
    }
  }
  return offspring;
}
}  // namespace neat
