#ifndef NEATLINK_H
#define NEATLINK_H

namespace neat {

/*!
 * @class Link
 *
 * @brief Represents a connection between two neurons in a NEAT neural network.
 *
 * @details Each link has a unique identifier, input and output neuron IDs, a weight, and an active state.
 */
class Link {
 public:
  explicit Link(int in_id, int out_id, double weight);

  int GetId() const;
  int GetInId() const;
  int GetOutId() const;
  double GetWeight() const;
  bool IsActive() const;

  void SetWeight(double weight);
  void SetActive();
  void SetInactive();

 private:
  static int next_id_; /*!< Static variable used to assign unique IDs to new links. */
  int id_; /*!< Unique identifier for the link. */
  int in_id_; /*!< ID of the input neuron. */
  int out_id_; /*!< ID of the output neuron. */
  double weight_; /*!< Weight of the connection. */
  bool active_; /*!< Indicates whether the link is active or not. */
};

Link CrossoverLink(const Link &a, const Link &b);
}  // namespace neat

#endif  // NEATLINK_H
