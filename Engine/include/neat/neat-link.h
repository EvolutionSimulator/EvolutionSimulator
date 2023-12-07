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
  void SetActive();
  void SetInactive();
 private:
  int id_;
  bool active_;
  double weight_;
  int in_id_;
  int out_id_;
};

Link CrossoverLink(const Link &a, const Link &b);
}  // namespace neat

#endif  // NEATLINK_H
