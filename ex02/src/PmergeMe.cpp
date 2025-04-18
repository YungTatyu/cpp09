#include "PmergeMe.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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

void PmergeMe::CreatePair(std::vector<PmergeNode> &v) const {
  v.reserve(list_.size() / 2);
  std::list<int>::const_iterator it = list_.begin();
  bool is_odd = list_.size() % 2 == 0;
  size_t end = is_odd ? list_.size() : list_.size() - 1;
  for (size_t i = 0; i < end; i += 2) {
    int first = *it;
    ++it;
    int second = *it;
    ++it;
    if (first > second) {
      v.push_back(PmergeNode(first, new PmergeNode(second)));

    } else {
      v.push_back(PmergeNode(second, new PmergeNode(first)));
    }
  }
  if (!is_odd) {
    v.push_back(PmergeNode(*it));
  }
}

std::vector<int> PmergeMe::MergeInsertionSortV() {
  std::vector<int> vv(list_.begin(), list_.end());
  std::vector<PmergeNode> v;
  CreatePair(v);
  for (std::vector<PmergeNode>::const_iterator it = v.begin(); it != v.end();
       ++it) {
    std::cout << it->bignum_ << " ";
  }
  std::cout << "\n";
  RecurMergeInsertionSort(v);
  return vv;
}

void PmergeMe::RecurMergeInsertionSort(const std::vector<PmergeNode> &v) {
  if (v.size() < 2) {
    return;
  }
  std::vector<PmergeNode> new_v;
  new_v.reserve(v.size() / 2);
  for (size_t i = 0; i + 1 < v.size(); i += 2) {
    if (v[i].bignum_ > v[i + 1].bignum_) {
      new_v.push_back(PmergeNode(v[i].bignum_, &v[i + 1]));
    } else {
      new_v.push_back(PmergeNode(v[i + 1].bignum_, &v[i]));
    }
  }
  if (v.size() % 2 != 0) {
    new_v.push_back(PmergeNode(v[v.size() - 1].bignum_));
  }
  for (std::vector<PmergeNode>::const_iterator it = new_v.begin();
       it != new_v.end(); ++it) {
    std::cout << it->bignum_ << " ";
  }
  std::cout << "\n";
  RecurMergeInsertionSort(new_v);
}
