#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <cstddef>
#include <deque>
#include <list>
#include <set>
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
  void push(PmergeNode *node) {
    pairs_.push(node);
    pairs_.pop();
  }
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
  PmergeMe(const std::list<std::string> &nums);
  ~PmergeMe();
  void SortAndPrint();
  std::vector<int> MergeInsertionSortV();
  std::deque<int> MergeInsertionSortQ();
  static const int jacob_stahal_seq[];

private:
  PmergeMe();
  PmergeMe(const PmergeMe &);
  PmergeMe &operator=(const PmergeMe &);
  void ParseNums(const std::list<std::string> &nums);
  void RecurMergeInsertionSort(std::vector<PmergeNode *> &v);
  void BinarySearchInsertion(ssize_t start, ssize_t end, PmergeNode *key);

  std::list<int> list_;
  std::set<PmergeNode *> v_inserted_;
  std::vector<PmergeNode *> v_sorted_;
  std::deque<PmergeNode> q_sorted_;
};

#endif // !PMERGE_ME_HPP
