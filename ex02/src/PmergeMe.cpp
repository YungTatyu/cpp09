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

const size_t PmergeMe::jacob_stahal_seq[] = {
    1UL * 2,
    1UL * 2,
    3UL * 2,
    5UL * 2,
    11UL * 2,
    21UL * 2,
    43UL * 2,
    85UL * 2,
    171UL * 2,
    341UL * 2,
    683UL * 2,
    1365UL * 2,
    2731UL * 2,
    5461UL * 2,
    10923UL * 2,
    21845UL * 2,
    43691UL * 2,
    87381UL * 2,
    174763UL * 2,
    349525UL * 2,
    699051UL * 2,
    1398101UL * 2,
    2796203UL * 2,
    5592405UL * 2,
    11184811UL * 2,
    22369621UL * 2,
    44739243UL * 2,
    89478485UL * 2,
    178956971UL * 2,
    357913941UL * 2,
    715827883UL * 2,
    1431655765UL * 2,
    2863311531UL * 2,
    5726623061UL * 2,
    11453246123UL * 2,
    22906492245UL * 2,
    45812984491UL * 2,
    91625968981UL * 2,
    183251937963UL * 2,
    366503875925UL * 2,
    733007751851UL * 2,
    1466015503701UL * 2,
    2932031007403UL * 2,
    5864062014805UL * 2,
    11728124029611UL * 2,
    23456248059221UL * 2,
    46912496118443UL * 2,
    93824992236885UL * 2,
    187649984473771UL * 2,
    375299968947541UL * 2,
    750599937895083UL * 2,
};

PmergeMe::PmergeMe() {}

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
    nums_.push_back(n);
  }
}

std::vector<int>
PmergeMe::MergeInsertionSortV(const std::list<int> *nums = NULL) {
  if (nums != NULL) {
    nums_ = *nums;
  }
  std::vector<PmergeNode *> v;
  v.reserve(nums_.size());
  for (std::list<int>::const_iterator it = nums_.begin(); it != nums_.end();
       ++it) {
    v.push_back(new PmergeNode(*it));
  }
  v_main_.reserve(nums_.size());
  RecurMergeInsertionSort(v);
  std::vector<int> re;
  re.reserve(nums_.size());
  for (size_t i = 0; i < v_main_.size(); ++i) {
    re.push_back(v_main_[i]->bignum_);
    delete v_main_[i];
  }
  return re;
}

void PmergeMe::RecurMergeInsertionSort(std::vector<PmergeNode *> &v) {
  if (v.size() < 2) {
    PmergeNode *p = v.front();
    v_main_.push_back(p->pop());
    v_main_.push_back(p);
    return;
  }
  std::vector<PmergeNode *> paired_v;
  paired_v.reserve(v.size() / 2);
  for (size_t i = 0; i + 1 < v.size(); i += 2) {
    if (v[i]->bignum_ > v[i + 1]->bignum_) {
      v[i]->push(v[i + 1]);
      paired_v.push_back(v[i]);
    } else {
      v[i + 1]->push(v[i]);
      paired_v.push_back(v[i + 1]);
    }
  }
  RecurMergeInsertionSort(paired_v);
  if (v.size() % 2 != 0) {
    // vのあまりを挿入
    BinarySearchInsertion(0, v_main_.size() - 1, v[v.size() - 1]);
  }
  if (v_main_.size() == nums_.size()) {
    return;
  }
  std::set<const PmergeNode *> pend_set;
  // 一番低い数字は飛ばす
  for (std::vector<PmergeNode *>::const_iterator it = v_main_.begin() + 1;
       it != v_main_.end(); ++it) {
    pend_set.insert(*it);
  }
  // 一番低い数字は左端で確定なのではじめに挿入
  v_main_.insert(v_main_.begin(), v_main_.front()->pop());

  size_t cur_index = 1; // 要素挿入のために進んだ一番右端のindex
  size_t jacob_i = 0;
  size_t cnt_inserted = 0;
  // pendをすべてmainに挿入する
  while (cnt_inserted < pend_set.size()) {
    cur_index =
        std::min(cur_index + jacob_stahal_seq[jacob_i], v_main_.size() - 1);
    size_t jacob_cnt =
        jacob_stahal_seq[jacob_i]; // countが0になるまで要素を挿入
    ++jacob_i;
    size_t reverse_i = cur_index; // 挿入するnodeのindex
    while (jacob_cnt != 0) {
      if (pend_set.find(v_main_[reverse_i]) == pend_set.end()) {
        --reverse_i;
        continue;
      }
      // pendのpairを挿入
      PmergeNode *inserting_node = v_main_[reverse_i]->pop();
      /*PmergeNode *inserting_node = v_sorted_[reverse_i];*/
      BinarySearchInsertion(0, reverse_i - 1, inserting_node);
      pend_set.erase(inserting_node);
      // 挿入するためindexが右にずれる
      ++cur_index;
      --jacob_cnt;
      ++cnt_inserted;
      if (cnt_inserted >= pend_set.size()) {
        break;
      }
    }
  }
}

void PmergeMe::BinarySearchInsertion(ssize_t start, ssize_t end,
                                     PmergeNode *key) {
  while (start <= end) {
    size_t middle = start + (end - start) / 2;
    if (*key < *v_main_[middle]) {
      end = middle - 1;
    } else {
      start = middle + 1;
    }
  }
  v_main_.insert(v_main_.begin() + start, key);
}

void PmergeMe::SortAndPrint() {
  std::vector<int> v = MergeInsertionSortV();
  for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << '\n';
}
