#include "neat/neat-genome.h"
/*!
 * @file neat-genome.h
 *
 * @brief Defines the Genome class used in NEAT (NeuroEvolution of Augmenting
 * Topologies).
 */

#include <algorithm>
#include <unordered_map>
#include <optional>
#include <random>
#include <set>
#include "settings.h"

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

void Genome::SetModules(const std::vector<BrainModule>& modules) {
  modules_ = modules;
}

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

  if (uniform(gen) < SETTINGS.neat.add_neuron_mutation_rate) {
    MutateAddNeuron();
  }

  if (uniform(gen) < SETTINGS.neat.add_link_mutation_rate) {
    MutateAddLink();
  }
  /* Removing things can mess up the cycles
    if (uniform(gen) < SETTINGS.neat.remove_neuron_mutation_rate) {
      MutateRemoveNeuron();
    }

    if (uniform(gen) < SETTINGS.neat.remove_link_mutation_rate) {
      MutateRemoveLink();
    }
  */
  if (uniform(gen) < SETTINGS.neat.change_weight_mutation_rate) {
    MutateChangeWeight();
  }

  if (uniform(gen) < SETTINGS.neat.change_bias_mutation_rate) {
    MutateChangeBias();
  }

  if (uniform(gen) < settings::neat::kModuleActivationMutationRate) {
      MutateActivateBrainModule();
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
  std::normal_distribution<> dis(0.0, SETTINGS.neat.standard_deviation_weight);

  for (Link& link : links_) {
    if (uniform(gen) < SETTINGS.neat.weight_mutation_rate) {
      double delta = dis(gen);
      link.SetWeight(link.GetWeight() + delta);

      if (link.GetWeight() > SETTINGS.neat.max_weight) {
        link.SetWeight(SETTINGS.neat.max_weight);
      } else if (link.GetWeight() < SETTINGS.neat.min_weight) {
        link.SetWeight(SETTINGS.neat.min_weight);
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
  std::normal_distribution<> dis(0.0, SETTINGS.neat.standard_deviation_weight);

  for (Neuron& neuron : neurons_) {
    if (uniform(gen) < SETTINGS.neat.bias_mutation_rate) {
      double delta = dis(gen);
      neuron.SetBias(neuron.GetBias() + delta);
      if (neuron.GetBias() > SETTINGS.neat.max_bias) {
        neuron.SetBias(SETTINGS.neat.max_bias);
      } else if (neuron.GetBias() < SETTINGS.neat.min_bias) {
        neuron.SetBias(SETTINGS.neat.min_bias);
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
    // link is not cyclic
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

  // Check if cycle exists:
  AddLink(Link(n1, n2, 1));
  if (DetectLoops(neurons_[indexRandomNeuron1])) {
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
  std::vector<BrainModule> modules_dominant = dominant.GetModules();
  offspring.SetModules(modules_dominant);

  // Return the new Genome that is a combination of both parents.
  return offspring;
}


/*!
 * @brief Randomly mutates the activation function of a hidden neuron in the Genome.
 *
 * This function selects a random hidden neuron and changes its activation function to a new type
 * randomly chosen from a predefined list of activation types.
 */
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

/*!
 * @brief Activates a random brain module from the available modules in the Genome.
 *
 * Adds a randomly selected brain module to the genome, updating input and output
 * neurons of the module to reflect the current state of the genome.
 */
void Genome::MutateActivateBrainModule(){

  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(0, AvailableModules.size() - 1);
  int randomIndex = distribution(generator);
  BrainModule module = AvailableModules.at(randomIndex);
  modules_.push_back(module);

  int input_size = module.GetInputNeuronIds().size();
  std::vector<int> input_ids(input_size, 0);
  for (int i = 0; i < module.GetInputNeuronIds().size(); i++) {
      if (i == 0) module.SetFirstInputIndex(GetInputCount());
      AddNeuron(Neuron(NeuronType::kInput, 0));
      input_ids.at(i) = neurons_.back().GetId();
  }
  module.SetInputNeuronIds(input_ids);

  int output_size = module.GetOutputNeuronIds().size();
  std::vector<int> output_ids(output_size, 0);
  for (int i = 0; i < module.GetOutputNeuronIds().size(); i++) {
      if (i == 0) module.SetFirstOutputIndex(GetOutputCount());
      AddNeuron(Neuron(NeuronType::kOutput, 0));
      output_ids.at(i) = neurons_.back().GetId();
  }
  module.SetOutputNeuronIds(output_ids);
}

/*!
 * @brief Disables a random brain module in the Genome.
 *
 * Removes a randomly selected brain module from the genome, including its associated
 * input and output neurons.
 */
void Genome::MutateDisableBrainModule(){
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(0, modules_.size() - 1);
  int randomIndex = distribution(generator);
  BrainModule module = modules_.at(randomIndex);
  modules_.erase(modules_.begin() + randomIndex);
  for (const int i : module.GetInputNeuronIds()){
    RemoveNeuron(i);
  }
  for (const int i : module.GetOutputNeuronIds()){
    RemoveNeuron(i);
  }
}

/*!
 * @brief Retrieves the list of modules currently in the Genome.
 *
 * @return A vector of BrainModule objects representing the modules currently in the genome.
 */
std::vector<BrainModule> Genome::GetModules() const { return modules_; }

/*!
 * @brief Finds a neuron in the Genome by its ID.
 *
 * @param targetId The ID of the neuron to find.
 * @param foundNeuron A reference to a Neuron object where the found neuron will be stored.
 * @return True if the neuron is found, false otherwise.
 */
bool Genome::FindNeuronById(int targetId, Neuron& foundNeuron) const{
    for (const Neuron& neuron : neurons_) {
        if (neuron.GetId() == targetId) {
            foundNeuron = neuron;  // Assign the found neuron to the reference parameter
            return true;  // Return true if the neuron is found
        }
    }
    return false;  // Return false if neuron isnt found
}

/*!
 * @brief Calculates the compatibility distance between this Genome and another
 * Genome.
 *
 * This method computes a distance based on the disjoint/excess neurons and
 * links, and weight difference between the two genomes, following a formula
 * from the paper.
 *
 * @param other A reference to another Genome object to compare with.
 * @return The calculated compatibility distance.
 */
double Genome::CompatibilityBetweenGenomes(const Genome& other) const {
    double average_weight_difference = 0;
    // Count shared neurons and their differences in bias
    // std::unordered_map<int, Neuron> shared_neurons;
    int Nshared_neurons = 0;  // number of neurons that appear in both genomes
    for (const Neuron& neuron : neurons_) {
        Neuron other_neuron(neuron.GetType(),
                            0);  // corresponding neuron of the other genome, if
                                 // it exists
        if (other.FindNeuronById(neuron.GetId(), other_neuron)) {
            Nshared_neurons++;
            // add a relative difference in biases (a number between 0 and 2)
            average_weight_difference +=
                abs(neuron.GetBias() - other_neuron.GetBias()) /
                std::max(abs(neuron.GetBias()), abs(other_neuron.GetBias()));
        }
    }

    // Count shared links and their weight differences
    // std::set<std::pair<int, int>> shared_link_pairs;
    // Genome& non_const_this = const_cast<Genome&>(*this);
    int Nshared_links = 0;
    for (const Link& link : links_) {
        for (const Link& other_link : other.GetLinks()) {
            if (link.GetId() == other_link.GetId()) {
        Nshared_links++;
        // add a relative difference in weights (a number between 0 and 2)
        average_weight_difference +=
            abs(link.GetWeight() - other_link.GetWeight()) /
            std::max(abs(link.GetWeight()), abs(other_link.GetWeight()));
        break;
            }
        }
    }
    average_weight_difference =
        average_weight_difference / (double)(Nshared_neurons + Nshared_links);

    int Ndisjoint_neurons =
        neurons_.size() + other.neurons_.size() - 2 * Nshared_neurons;
    int normalizing_factor_neurons =
        std::max(neurons_.size(), other.neurons_.size());
    double normalized_disjoint_neurons =
        (double)Ndisjoint_neurons / (double)normalizing_factor_neurons;

    int Ndisjoint_links =
        links_.size() + other.links_.size() - 2 * Nshared_links;
    int normalizing_factor_links = std::max(links_.size(), other.links_.size());
    double normalized_disjoint_links =
        (double)Ndisjoint_links / (double)normalizing_factor_links;
    // Compute compatibility distance based on disjoint/excess neurons and
    // links, and average weight difference.
    double compatibility_distance =
        settings::compatibility::kWeightSharedNeurons *
            normalized_disjoint_neurons +
        settings::compatibility::kWeightSharedLinks *
            normalized_disjoint_links +
        settings::compatibility::kAverageWeightSharedLinks *
            average_weight_difference;
    return compatibility_distance;
}

Genome minimallyViableGenome() {
  int start_id = neat::Neuron::next_id_ - 1;
  Genome genome(SETTINGS.environment.input_neurons,
                SETTINGS.environment.output_neurons);
  Link constant_acceleration(
      start_id + 1, start_id + SETTINGS.environment.input_neurons + 1, 1);
  genome.AddLink(constant_acceleration);
  Link digest(start_id + 1, start_id + SETTINGS.environment.input_neurons + 6,
              1);
  Link stop_digesting(start_id + 6,
                      start_id + SETTINGS.environment.input_neurons + 6, -1);
  genome.AddLink(digest);
  genome.AddLink(stop_digesting);
  Link orient_towards_food(
      start_id + 7, start_id + SETTINGS.environment.input_neurons + 3, 1);
  genome.AddLink(orient_towards_food);
  Neuron BiteManager(NeuronType::kHidden, 1);
  BiteManager.SetActivation(ActivationType::sigmoid);
  int id = BiteManager.GetId();
  genome.AddNeuron(BiteManager);
  Link distance_bite(start_id + 8, id, -1);
  Link activation_bite(id, start_id + SETTINGS.environment.input_neurons + 5,
                       1);
  genome.AddLink(distance_bite);
  genome.AddLink(activation_bite);
  Link slow_rotation(start_id + 5,
                     start_id + SETTINGS.environment.input_neurons + 3, -0.1);
  genome.AddLink(slow_rotation);
  return genome;
}
}  // namespace neat
