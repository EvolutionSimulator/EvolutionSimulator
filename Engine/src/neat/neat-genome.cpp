#include "neat/neat-genome.h"
#include "neat/neat-neuron.h"
/*!
 * @file neat-genome.h
 *
 * @brief Defines the Genome class used in NEAT (NeuroEvolution of Augmenting
 * Topologies).
 */

#include <algorithm>
#include <optional>
#include <random>

namespace neat {

/*!
 * @brief Constructs a Genome with a specified number of input and output
 * neurons.
 *
 * @param input_count Number of input neurons.
 * @param output_count Number of output neurons.
 */
Genome::Genome(int input_count, int output_count) : neurons_(), links_() {
  for (int i = 0; i < input_count; ++i) {
    AddNeuron(Neuron(NeuronType::kInput, 0.0));
  }
  for (int i = 0; i < output_count; ++i) {
    AddNeuron(Neuron(NeuronType::kOutput, 0.0));
  }
}

/*!
 * @brief Gets the number of input neurons in the Genome.
 *
 * @return The count of input neurons.
 */
int Genome::GetInputCount() const {
  return std::count_if(neurons_.begin(), neurons_.end(),
                       [](const Neuron& neuron) {
                         return neuron.GetType() == NeuronType::kInput;
                       });
}

/*!
 * @brief Gets the number of output neurons in the Genome.
 *
 * @return The count of output neurons.
 */
int Genome::GetOutputCount() const {
  return std::count_if(neurons_.begin(), neurons_.end(),
                       [](const Neuron& neuron) {
                         return neuron.GetType() == NeuronType::kOutput;
                       });
}

/*!
 * @brief Gets the list of neurons in the Genome.
 *
 * @return A constant reference to the vector of Neurons.
 */
const std::vector<Neuron>& Genome::GetNeurons() const { return neurons_; }

/*!
 * @brief Gets the list of links (connections) in the Genome.
 *
 * @return A constant reference to the vector of Links.
 */
const std::vector<Link>& Genome::GetLinks() const { return links_; }

/*!
 * @brief Adds a neuron to the Genome.
 *
 * @param neuron The Neuron to be added.
 */
void Genome::AddNeuron(const Neuron& neuron) { neurons_.push_back(neuron); }

/*!
 * @brief Adds a link (connection) between neurons in the Genome.
 *
 * @param link The Link to be added.
 */
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

/*!
 * @brief Disables a link in the Genome by its ID.
 *
 * @param id The unique identifier of the link to disable.
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

/*!
 * @brief Enables a previously disabled link in the Genome by its ID.
 *
 * @param id The unique identifier of the link to enable.
 */
void Genome::EnableLink(int id) {
  for (Link& link : links_) {
    if (link.GetId() == id) {
      link.SetActive();
    }
  }
}

/*!
 * @brief Removes a neuron and its associated links from the Genome by its ID.
 *
 * @param id The unique identifier of the neuron to remove.
 */
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
    for (int i = links_.size() - 1; i >= 0; i--) {
      if (neurons_[index_to_delete].GetId() == links_[i].GetInId() ||
          neurons_[index_to_delete].GetId() == links_[i].GetOutId()) {
        links_to_remove.push_back(i);
      }
    }

    for (int i : links_to_remove) {
      links_.erase(links_.begin() + i);
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

/*!
 * @brief Mutates the Genome, potentially altering its structure and weights.
 *
 * @details This method randomly applies various mutation operations based on
 * predefined probabilities.
 */
void Genome::Mutate() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  if (uniform(gen) < settings::neat::kAddNeuronMutationRate) {
    MutateAddNeuron();
  }

  if (uniform(gen) < settings::neat::kAddLinkMutationRate) {
    MutateAddLink();
  }
  /* cycles aren't supported with remove mutations
    if (uniform(gen) < settings::neat::kRemoveNeuronMutationRate) {
      MutateRemoveNeuron();
    }/*

  /*  if (uniform(gen) < settings::neat::kRemoveLinkMutationRate) {
      MutateRemoveLink();
    }*/

  if (uniform(gen) < settings::neat::kChangeWeightMutationRate) {
    MutateChangeWeight();
  }

  if (uniform(gen) < settings::neat::kChangeBiasMutationRate) {
    MutateChangeBias();
  }

  if (uniform(gen) < settings::neat::kActivationFunctionMutationRate) {
      MutateActivationFunction();
    }
}

/*!
 * @brief Mutates the Genome by removing a neuron.
 *
 * @details Randomly selects and removes a neuron from the Genome, along with
 * its associated links.
 */
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

/*!
 * @brief Mutates the Genome by removing a link.
 *
 * @details Randomly selects and removes a link between neurons in the Genome.
 */
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

/*!
 * @brief Checks if a link exists between two neurons in the Genome.
 *
 * @param inID The unique identifier of the input neuron.
 * @param outID The unique identifier of the output neuron.
 *
 * @return True if the link exists, false otherwise.
 */
bool Genome::HasLink(const int& inID, const int& outID) {
  for (const auto& link : links_) {
    if (link.GetInId() == inID && link.GetOutId() == outID ||
        link.GetInId() == outID && link.GetOutId() == inID) {
      return true;
    }
  }
  return false;
}

/*!
 * @brief Removes a link from the Genome by its ID.
 *
 * @param id The unique identifier of the link to remove.
 */
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

/*!
 * @brief Mutates the Genome by changing the weights of its links.
 *
 * @details Adjusts the weights of links in the Genome based on a normal
 * distribution.
 */
void Genome::MutateChangeWeight() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);
  std::normal_distribution<> dis(0.0, settings::neat::kStandardDeviationWeight);

  for (Link& link : links_) {
    if (uniform(gen) < settings::neat::kWeightMutationRate) {
      double delta = dis(gen);
      link.SetWeight(link.GetWeight() + delta);

      if (link.GetWeight() > settings::neat::kMaxWeight) {
        link.SetWeight(settings::neat::kMaxWeight);
      } else if (link.GetWeight() < settings::neat::kMinWeight) {
        link.SetWeight(settings::neat::kMinWeight);
      }
    }
  }
}

/*!
 * @brief Mutates the Genome by changing the bias values of its neurons.
 *
 * @details Adjusts the bias values of neurons in the Genome based on a normal
 * distribution.
 */
void Genome::MutateChangeBias() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform(0.0, 1.0);
  std::normal_distribution<> dis(0.0, settings::neat::kStandardDeviationWeight);

  for (Neuron& neuron : neurons_) {
    if (uniform(gen) < settings::neat::kBiasMutationRate) {
      double delta = dis(gen);
      neuron.SetBias(neuron.GetBias() + delta);
      if (neuron.GetBias() > settings::neat::kMaxBias) {
        neuron.SetBias(settings::neat::kMaxBias);
      } else if (neuron.GetBias() < settings::neat::kMinBias) {
        neuron.SetBias(settings::neat::kMinBias);
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

/*!
 * @brief Performs a Depth-First Search (DFS) to detect cycles in the Genome.
 *
 * @details This function is used internally to ensure the neural network
 * represented by the Genome does not contain cycles.
 *
 * @param currentNeuron The neuron to start the DFS from.
 * @param visited A set of neuron IDs that have already been visited.
 * @param visiting A set of neuron IDs currently being visited.
 *
 * @return True if a cycle is detected, false otherwise.
 */
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
    if (link.IsCyclic()) {
      continue;
    }
    //link is not cyclic
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

/*!
 * @brief Detects cycles in the Genome starting from a specified neuron.
 *
 * @param startNeuron The neuron from which to start the cycle detection.
 *
 * @return True if a cycle is detected, false otherwise.
 */
bool Genome::DetectLoops(const Neuron& startNeuron) {
  std::unordered_set<int> visited;
  std::unordered_set<int> visiting;

  return DFS(startNeuron, visited, visiting);
}

/*!
 * @brief Mutates the Genome by adding a new link between neurons.
 *
 * @details Adds a new link between two randomly chosen neurons.
 * If this creates a cycle, the added link is characterized as cyclic
 * and its parameter cyclic_ is set to true.
 */
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

  AddLink(Link(n1, n2, 1));
  // Check if cycle exists:

  if (DetectLoops(neurons_[indexRandomNeuron1])) {
    //RemoveLink(newl.GetId());
    links_.back().SetCyclic();
    return;
  }
}

/*!
 * @brief Mutates the Genome by adding a new neuron.
 *
 * @details Adds a new neuron by splitting an existing link and connecting the
 * new neuron in between.
 */
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
  Link newlink1(RandomLink.GetInId(), newNeuronId, 1);
  Link newlink2(newNeuronId, RandomLink.GetOutId(), RandomLink.GetWeight());
  if (RandomLink.IsCyclic()) {
    newlink2.SetCyclic();
  }
  AddLink(newlink1);
  AddLink(newlink2);
}

/*!
 * @brief Performs crossover between two Genomes to produce an offspring Genome.
 *
 * @param dominant The dominant Genome in the crossover.
 * @param recessive The recessive Genome in the crossover.
 *
 * @return A new Genome resulting from the crossover of the two parent Genomes.
 */
Genome Crossover(const Genome& dominant, const Genome& recessive) {
  // Initialize the offspring Genome with no inputs and outputs initially.
  Genome offspring{0, 0};

  // Iterate over the neurons in the dominant Genome.
  for (const auto& dominant_neuron : dominant.GetNeurons()) {
    int neuron_id = dominant_neuron.GetId();
    std::optional<Neuron> recessive_neuron;

    // Search for the corresponding neuron in the recessive Genome.
    for (const auto& recessive_neuron_candidate : recessive.GetNeurons()) {
      if (recessive_neuron_candidate.GetId() == neuron_id) {
        recessive_neuron = recessive_neuron_candidate;
        break;
      }
    }

    // If the neuron is not found in the recessive Genome, add it to the
    // offspring.
    if (!recessive_neuron) {
      offspring.AddNeuron(dominant_neuron);
    } else {
      // If the neuron is found, combine the properties from both parents.
      offspring.AddNeuron(
          CrossoverNeuron(dominant_neuron, recessive_neuron.value()));
    }
  }

  // Similar process for links - iterate over links in the dominant Genome.
  for (const auto& dominant_link : dominant.GetLinks()) {
    int link_id = dominant_link.GetId();
    std::optional<Link> recessive_link;

    // Search for the corresponding link in the recessive Genome.
    for (const auto& recessive_link_candidate : recessive.GetLinks()) {
      if (recessive_link_candidate.GetId() == link_id) {
        recessive_link = recessive_link_candidate;
        break;
      }
    }

    // If the link is not found in the recessive Genome, add it to the
    // offspring.
    if (!recessive_link) {
      offspring.AddLink(dominant_link);
    } else {
      // If the link is found, combine the properties from both parents.
      offspring.AddLink(CrossoverLink(dominant_link, recessive_link.value()));
    }
  }

  // Return the new Genome that is a combination of both parents.
  return offspring;
}


void Genome::MutateActivationFunction() {
    if (GetInputCount()+GetOutputCount()==neurons_.size()){
        return ;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, neurons_.size() - 1);
    size_t indexRandomNeuronHidden = dist(gen);
    while (neurons_[indexRandomNeuronHidden].GetType() != NeuronType::kHidden) {
      indexRandomNeuronHidden = dist(gen);
    }
    std::vector<ActivationType> activationTypes = {
            ActivationType::sigmoid,
            ActivationType::relu,
            ActivationType::elu,
            ActivationType::leakyRelu,
            ActivationType::binary,
            ActivationType::linear
        };
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, activationTypes.size() - 1);
    int randomIndex = distribution(generator);
    neurons_[indexRandomNeuronHidden].SetActivation(activationTypes[randomIndex]);
}

bool Genome::FindNeuronById(int targetId, Neuron& foundNeuron) const{
    for (const Neuron& neuron : neurons_) {
        if (neuron.GetId() == targetId) {
            foundNeuron = neuron;  // Assign the found neuron to the reference parameter
            return true;  // Return true if the neuron is found
        }
    }
    return false;  // Return false if neuron isnt found
}
}  // namespace neat
