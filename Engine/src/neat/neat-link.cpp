#include "neat/neat-link.h"

namespace neat {

Link::Link(int id, int in_id, int out_id, double weight)
    : id_(id), in_id_(in_id), out_id_(out_id), weight_(weight), active_(true) {}

int Link::GetId() const { return id_; }

int Link::GetInId() const { return in_id_; }

int Link::GetOutId() const { return out_id_; }

double Link::GetWeight() const { return weight_; }

bool Link::IsActive() const { return active_; }

void Link::SetWeight(double weight) { weight_ = weight; }

void Link::SetActive(){active_= true;}

void Link::SetInactive(){active_ = false;}

}  // namespace neat
