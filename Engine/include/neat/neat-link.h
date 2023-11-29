#ifndef NEATLINK_H
#define NEATLINK_H

namespace neat {

class Link {
 public:
  Link(int id, int in_id, int out_id, double weight);

  int GetId() const;
  int GetInId() const;
  int GetOutId() const;
  double GetWeight() const;
  bool IsActive() const;

  void SetWeight(double weight);

 private:
  int id_;
  double weight_;
  int in_id_;
  int out_id_;
  bool active_;
};

}  // namespace neat

#endif  // NEATLINK_H
