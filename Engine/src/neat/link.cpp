#include "neat/link.h"
/*!
 * @file neat-link.h
 *
 * @brief Defines the Link class and related functions for NEAT.
 */

#include "core/random.h"
#include "stdexcept"

namespace neat {

int Link::next_id_ = 1;

/*!
 * @brief Constructs a Link with specified input and output neuron IDs and
 * weight.
 *
 * @param in_id The ID of the input neuron.
 * @param out_id The ID of the output neuron.
 * @param weight The weight of the connection.
 */
Link::Link(int in_id, int out_id, double weight)
    : id_(next_id_++),
      in_id_(in_id),
      out_id_(out_id),
      weight_(weight),
      active_(true),
      cyclic_(false) {}

Link::Link(int id, int in_id, int out_id, double weight, bool active,
           bool cyclic)
    : id_(id),
      in_id_(in_id),
      out_id_(out_id),
      weight_(weight),
      active_(active),
      cyclic_(cyclic) {}
      
/*!
 * @brief Gets the unique identifier of the link.
 *
 * @return The ID of the link.
 */

int Link::GetId() const { return id_; }

/*!
 * @brief Gets the ID of the input neuron.
 *
 * @return The input neuron ID.
 */
int Link::GetInId() const { return in_id_; }

/*!
 * @brief Gets the ID of the output neuron.
 *
 * @return The output neuron ID.
 */
int Link::GetOutId() const { return out_id_; }

/*!
 * @brief Gets the weight of the link.
 *
 * @return The weight of the connection.
 */
double Link::GetWeight() const { return weight_; }

/*!
 * @brief Checks if the link is active.
 *
 * @return True if the link is active, false otherwise.
 */
bool Link::IsActive() const { return active_; }

/*!
 * @brief Checks if the link is cyclic.
 *
 * @return True if the link is cyclic, false otherwise.
 */
bool Link::IsCyclic() const { return cyclic_; }

/*!
 * @brief Sets the weight of the link.
 *
 * @param weight The new weight to be set for the connection.
 */
void Link::SetWeight(double weight) { weight_ = weight; }

/*!
 * @brief Sets the link to be active.
 */
void Link::SetActive() { active_ = true; }

/*!
 * @brief Sets the link to be inactive.
 */
void Link::SetInactive() { active_ = false; }

/*!
 * @brief Sets the link to be cyclic.
 */
void Link::SetCyclic() { cyclic_ = true; }

/*!
 * @brief Sets the link to be non-cyclic.
 */
void Link::SetNonCyclic() { cyclic_ = false; }

/*!
 * @brief Creates a new Link with specified input and output neuron IDs and
 * weight.
 *
 * @param in_id The ID of the input neuron.
 * @param out_id The ID of the output neuron.
 * @param weight The weight of the connection.
 *
 * @return A new Link object.
 */
Link NewLink(int in_id, int out_id, double weight);

/*!
 * @brief Performs crossover between two Links to produce a new Link.
 *
 * @param a The first Link in the crossover.
 * @param b The second Link in the crossover.
 * @return A new Link resulting from the crossover of the two links.
 *
 * @throws std::invalid_argument if the two links have different IDs.
 *
 * @details The crossover Link's weight is randomly chosen from either 'a' or
 * 'b'.
 */
Link CrossoverLink(const Link &a, const Link &b) {
  if (a.GetId() != b.GetId()) {
    throw std::invalid_argument("Links must have the same id");
  }

  Link crossover_link = a;

  crossover_link.SetWeight(Random::Double(0.0, 1.0) < 0.5 ? a.GetWeight()
                                                   : b.GetWeight());

  return crossover_link;
}

}  // namespace neat
