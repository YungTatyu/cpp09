#include "PmergeMe.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <list>
#include <set>
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
  std::vector<PmergeNode> pend;
  pend.reserve(v.size() / 2);
  for (size_t i = 0; i + 1 < v.size(); i += 2) {
    if (v[i].bignum_ > v[i + 1].bignum_) {
      v[i].push(&v[i + 1]);
      pend.push_back(v[i]);
    } else {
      v[i].push(&v[i]);
      pend.push_back(v[i + 1]);
    }
  }
  /*for (std::vector<PmergeNode>::const_iterator it = paired.begin();*/
  /*     it != paired.end(); ++it) {*/
  /*  std::cout << it->bignum_ << " ";*/
  /*}*/
  /*std::cout << "\n";*/
  RecurMergeInsertionSort(pend);
  // 一番低い数字は左端で確定なのではじめに挿入
  v_sorted_.insert(pend.begin(), pend.front());

  std::set<const PmergeNode *> pend_set;
  for (std::vector<PmergeNode>::const_iterator it = pend.begin() + 1;
       it != pend.end(); ++it) {
    pend_set.insert(&(*it));
  }

  size_t cur_index = 1; // 要素挿入のために進んだ一番右端のindex
  size_t jacob_i = 0;
  // pendをすべてmainに挿入する
  for (size_t i = 0; i != pend.size(); ++i) {
    cur_index =
        std::min(cur_index + jacob_stahal_seq[jacob_i], v_sorted_.size() - 1);
    size_t jacob_cnt =
        jacob_stahal_seq[jacob_i]; // countが0になるまで要素を挿入
    ++jacob_i;
    size_t reverse_i = cur_index; // 挿入するnodeのindex
    size_t reverse_cnt = 0;       // 挿入しなかったnodeのcount
    while (jacob_cnt != 0) {
      const PmergeNode *inserting_node = v_sorted_[cur_index].pop();
      if (pend_set.find(inserting_node) == pend_set.end()) {
        --reverse_i;
        ++reverse_cnt;
        continue;
      }
    }
  }
  if (v.size() % 2 != 0) {
    // vのあまりを挿入
  }
}

/*1 2 3 4 5 8 9 10*/
void PmergeMe::BinarySearchInsertion(size_t start, size_t end,
                                     const PmergeNode *key) {
  while (start <= end) {
    size_t middle = (start + end) / 2;
    if (*key > v_sorted_[middle]) {
      start = middle + 1;
    } else {
      end = middle - 1;
    }
  }
}
