#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <cstddef>
#include <deque>
#include <list>
#include <stack>
#include <string>
#include <vector>

class PmergeNode {
public:
  PmergeNode(int v) { bignum_ = v; }
  PmergeNode(const PmergeNode &other) { *this = other; }
  PmergeNode &operator=(const PmergeNode &other) {
    if (this != &other) {
      bignum_ = other.bignum_;
      pairs_ = other.pairs_;
    }
    return *this;
  }
  ~PmergeNode() {}
  bool operator>(const PmergeNode &other) const {
    return bignum_ > other.bignum_;
  }
  bool operator<(const PmergeNode &other) const {
    return bignum_ < other.bignum_;
  }
  void push(const PmergeNode *node) {
    pairs_.push(node);
    pairs_.pop();
  }
  const PmergeNode *pop() {
    const PmergeNode *re = pairs_.top();
    pairs_.pop();
    return re;
  }

  int bignum_;
  std::stack<const PmergeNode *> pairs_;

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
  static const int jacob_stahal_seq[];

private:
  PmergeMe();
  PmergeMe(const PmergeMe &);
  PmergeMe &operator=(const PmergeMe &);
  void ParseNums(const std::list<std::string> &nums);
  void RecurMergeInsertionSort(std::vector<PmergeNode> &v);
  void BinarySearchInsertion(size_t start, size_t end, const PmergeNode *key);

  std::list<int> list_;
  std::vector<PmergeNode> v_sorted_;
  std::deque<PmergeNode> q_sorted_;
};

#endif // !PMERGE_ME_HPP
