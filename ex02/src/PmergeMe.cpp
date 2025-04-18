#include "PmergeMe.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const int PmergeMe::jacob_stahal_seq[] = {
    2, 2, 6, 10, 22, 42, 86, 170, 342, 682, 1366, 2730, 5462,
};

PmergeMe::PmergeMe(const std::list<std::string> &nums) {
  this->ParseNums(nums);
}

PmergeMe::~PmergeMe() {}

void PmergeMe::ParseNums(const std::list<std::string> &nums) {
  for (std::list<std::string>::const_iterator it = nums.begin();
       it != nums.end(); ++it) {
    std::stringstream ss(*it);
    int n;
    ss >> n;
    if (ss.fail() || !ss.eof()) {
      throw std::runtime_error(std::string("error: invalid input ") + *it);
    }
    list_.push_back(n);
  }
}

std::vector<int> PmergeMe::MergeInsertionSortV() {
  std::vector<int> vv(list_.begin(), list_.end());
  std::vector<PmergeNode> v;
  v.reserve(list_.size());
  for (std::list<int>::const_iterator it = list_.begin(); it != list_.end();
       ++it) {
    v.push_back(PmergeNode(*it));
  }
  RecurMergeInsertionSort(v);
  return vv;
}

void PmergeMe::RecurMergeInsertionSort(std::vector<PmergeNode> &v) {
  if (v.size() < 2) {
    return;
  }
  std::vector<PmergeNode> paired;
  paired.reserve(v.size() / 2);
  for (size_t i = 0; i + 1 < v.size(); i += 2) {
    if (v[i].bignum_ > v[i + 1].bignum_) {
      v[i].push(&v[i + 1]);
      paired.push_back(v[i]);
    } else {
      v[i].push(&v[i]);
      paired.push_back(v[i + 1]);
    }
  }
  /*for (std::vector<PmergeNode>::const_iterator it = paired.begin();*/
  /*     it != paired.end(); ++it) {*/
  /*  std::cout << it->bignum_ << " ";*/
  /*}*/
  /*std::cout << "\n";*/
  RecurMergeInsertionSort(paired);
  v_sorted_.insert(paired.begin(), paired.front());
  std::vector<PmergeNode> pend;
  for (std::vector<PmergeNode>::const_iterator it = paired.begin() + 1;
       it != paired.end(); ++it) {
    pend.push_back(*it);
  }
  if (v.size() % 2 != 0) {
    pend.push_back(v[v.size() - 1]);
  }
}
