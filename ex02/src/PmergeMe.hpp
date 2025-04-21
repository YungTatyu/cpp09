#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <deque>
#include <list>
#include <stack>
#include <string>
#include <sys/types.h>
#include <vector>

class PmergeNode {
public:
  PmergeNode(int v) { bignum_ = v; }
  ~PmergeNode() {}
  bool operator>(const PmergeNode &other) const {
    return bignum_ > other.bignum_;
  }
  bool operator<(const PmergeNode &other) const {
    return bignum_ < other.bignum_;
  }
  void push(PmergeNode *node) { pairs_.push(node); }

  PmergeNode *pop() {
    PmergeNode *re = pairs_.top();
    pairs_.pop();
    return re;
  }

  int bignum_;
  std::stack<PmergeNode *> pairs_;

private:
  PmergeNode();
  PmergeNode(const PmergeNode &other) { *this = other; }
  PmergeNode &operator=(const PmergeNode &other) {
    if (this != &other) {
      bignum_ = other.bignum_;
      pairs_ = other.pairs_;
    }
    return *this;
  }
};

class PmergeMe {
public:
  PmergeMe();
  PmergeMe(const std::list<std::string> &nums);
  ~PmergeMe();
  void SortAndPrint();
  std::vector<int> MergeInsertionSortV(const std::list<int> *nums);
  std::deque<int> MergeInsertionSortDq(const std::list<int> *nums);

private:
  PmergeMe(const PmergeMe &);
  PmergeMe &operator=(const PmergeMe &);
  void ParseNums(const std::list<std::string> &nums);
  void RecurMergeInsertionSort(std::vector<PmergeNode *> &v);
  void BinarySearchInsertion(ssize_t start, ssize_t end, PmergeNode *key);
  double CalcElapsedus(const timeval &start, const timeval &end);

  std::list<int> nums_;
  std::vector<PmergeNode *> v_main_;
  std::deque<PmergeNode> dq_main_;
  static const size_t jacob_stahal_seq[];
};

#endif // !PMERGE_ME_HPP
