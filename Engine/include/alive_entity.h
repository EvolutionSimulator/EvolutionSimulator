#ifndef ALIVEENTITY_H
#define ALIVEENTITY_H

#include "entity.h"
#include "mutable.h"
#include "neat/neat-neural-network.h"

class AliveEntity : virtual public Entity {
 public:
  AliveEntity(neat::Genome genome, Mutable mutable_);
  virtual ~AliveEntity() override {}

  void Dies();
  double GetHealth() const;
  void SetHealth(double health);

  double GetEnergy() const;
  void SetEnergy(double energy);

  double GetMaxEnergy() const;
  void SetMaxEnergy(double max_energy);
  void UpdateMaxEnergy();

  void BalanceHealthEnergy();

  double GetAge() const;
  void SetAge(double age);
  void UpdateAge(double delta_time);

  neat::Genome GetGenome() const;
  Mutable GetMutable() const;

  int GetGeneration() const;
  void SetGeneration(int generation);

 protected:
  double max_energy_;
  double energy_; /*!< Stores the current energy level of the creature. */

  double health_; /*!< Represents the current health status of the creature. */

  double age_; /*!< Tracks the age of the creature. */

  Mutable mutable_;

  neat::NeuralNetwork brain_; /*!< Neural network for processing environmental
                                 stimuli and decision making. */
  neat::Genome genome_;       /*!< Genetic makeup of the creature. */
  std::vector<double>
      neuron_data_; /*!< Neuron data used in the neural network. */

  int generation_ = 0; /*!< Generation count of the creature. */
};

#endif  // ALIVEENTITY_H
