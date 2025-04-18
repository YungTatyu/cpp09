#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <deque>
#include <list>
#include <string>
#include <vector>

class PmergeNode {
public:
  PmergeNode(int v, const PmergeNode *pair = NULL) {
    bignum_ = v;
    pair_ = pair;
  }
  PmergeNode(const PmergeNode &other) { *this = other; }
  PmergeNode &operator=(const PmergeNode &other) {
    if (this != &other) {
      bignum_ = other.bignum_;
      pair_ = other.pair_;
    }
    return *this;
  }
  ~PmergeNode() {}
  bool operator>(const PmergeNode &other) { return bignum_ > other.bignum_; }
  bool operator<(const PmergeNode &other) { return bignum_ < other.bignum_; }

  int bignum_;
  const PmergeNode *pair_;

private:
  PmergeNode();
};

class PmergeMe {
public:
  PmergeMe(const std::list<std::string> &nums);
  ~PmergeMe();
  void SortAndPrint();
  std::vector<int> MergeInsertionSortV();
  std::deque<int> MergeInsertionSortQ();

private:
  PmergeMe();
  PmergeMe(const PmergeMe &);
  PmergeMe &operator=(const PmergeMe &);
  void ParseNums(const std::list<std::string> &nums);
  void CreatePair(std::vector<PmergeNode> &v) const;
  void RecurMergeInsertionSort(const std::vector<PmergeNode> &v);

  std::list<int> list_;
  std::vector<PmergeNode> v_sorted_;
  std::deque<PmergeNode> q_sorted_;
};

#endif // !PMERGE_ME_HPP
