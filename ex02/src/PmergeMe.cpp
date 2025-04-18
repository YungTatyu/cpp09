#include "PmergeMe.hpp"
#include <iostream>
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
  std::list<int>::const_iterator it = list_.begin();
  v.reserve(list_.size() / 2);
  while (it != list_.end()) {
    int first = *it;
    ++it;
    int second = *it;
    if (first > second) {
      v.push_back(PmergeNode(first, new PmergeNode(second)));

    } else {
      v.push_back(PmergeNode(second, new PmergeNode(first)));
    }
  }
}
