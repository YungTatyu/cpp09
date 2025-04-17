#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <deque>
#include <list>
#include <string>
#include <vector>

class PmergeMe {
public:
  PmergeMe(const std::list<std::string> &nums);
  ~PmergeMe();
  void SortAndPrint();
  std::vector<int> MergeInsertionSort(const std::vector<int> &v);
  std::deque<int> MergeInsertionSort(const std::deque<int> &q);

private:
  PmergeMe();
  PmergeMe(const PmergeMe &);
  PmergeMe &operator=(const PmergeMe &);
  void ParseNums(const std::list<std::string> &nums);
  std::list<int> list_;
};

#endif // !PMERGE_ME_HPP
