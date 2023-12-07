#include "neat/neat-link.h"
#include "assert.h"
#include "random"

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

Link CrossoverLink(const Link &a, const Link &b){
    assert(a.GetId() == b.GetId());

    int id = a.GetId();
    int id_in = a.GetInId();
    int id_out = a.GetOutId();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double weight = distribution(gen) < 0.5 ? a.GetWeight() : b.GetWeight();

    return {id, id_in, id_out, weight};
}
}  // namespace neat
