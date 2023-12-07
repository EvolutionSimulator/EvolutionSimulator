#include "neat/neat-link.h"

#include "random"

namespace neat {

int Link::next_id_ = 1;

Link::Link(int in_id, int out_id, double weight)
    : id_(next_id_++),
      in_id_(in_id),
      out_id_(out_id),
      weight_(weight),
      active_(true) {}

int Link::GetId() const { return id_; }

int Link::GetInId() const { return in_id_; }

int Link::GetOutId() const { return out_id_; }

double Link::GetWeight() const { return weight_; }

bool Link::IsActive() const { return active_; }

void Link::SetWeight(double weight) { weight_ = weight; }

void Link::SetActive() { active_ = true; }

void Link::SetInactive() { active_ = false; }

Link NewLink(int in_id, int out_id, double weight);

Link CrossoverLink(const Link &a, const Link &b) {
  if (a.GetId() != b.GetId()) {
    throw std::invalid_argument("Links must have the same id");
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  Link crossover_link = a;

  crossover_link.SetWeight(distribution(gen) < 0.5 ? a.GetWeight()
                                                   : b.GetWeight());

  return crossover_link;
}

}  // namespace neat
