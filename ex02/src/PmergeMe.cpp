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
void PmergeMe::SortAndPrint() {
  std::vector<int> v = MergeInsertionSortV();
  for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << '\n';
}

std::vector<int> PmergeMe::MergeInsertionSortV() {
  std::vector<PmergeNode *> v;
  v.reserve(list_.size());
  // WARN: これがないとメモリが再確保されて壊れる
  v_sorted_.reserve(list_.size());
  for (std::list<int>::const_iterator it = list_.begin(); it != list_.end();
       ++it) {
    v.push_back(new PmergeNode(*it));
    std::cout << *it << " ";
  }
  std::cout << "init values\n";
  RecurMergeInsertionSort(v);
  std::vector<int> re;
  re.reserve(list_.size());
  for (size_t i = 0; i < v.size(); ++i) {
    re.push_back(v[i]->bignum_);
  }
  return re;
}

void PmergeMe::RecurMergeInsertionSort(std::vector<PmergeNode *> &v) {
  if (v.size() < 2) {
    return;
  }
  std::vector<PmergeNode *> pend;
  pend.reserve(v.size() / 2);
  for (size_t i = 0; i + 1 < v.size(); i += 2) {
    if (v[i]->bignum_ > v[i + 1]->bignum_) {
      v[i]->push(v[i + 1]);
      pend.push_back(v[i]);
    } else {
      v[i + 1]->push(v[i]);
      pend.push_back(v[i + 1]);
    }
  }
  for (std::vector<PmergeNode *>::const_iterator it = pend.begin();
       it != pend.end(); ++it) {
    std::cout << (*it)->bignum_ << " ";
  }
  std::cout << "\n";
  RecurMergeInsertionSort(pend);
  std::cout << "\n\npending: ";
  for (std::vector<PmergeNode *>::const_iterator it = pend.begin();
       it != pend.end(); ++it) {
    std::cout << (*it)->bignum_ << " ";
  }
  std::cout << "\n";
  std::cout << pend.size() << ": pend size\n";
  std::cout << pend.front()->bignum_ << ": num\n";
  // 一番低い数字は左端で確定なのではじめに挿入
  v_sorted_.insert(v_sorted_.begin(), pend.front());
  /*pend.erase(pend.begin());*/
  std::cout << pend.front()->bignum_ << ": inserted front\n";

  std::set<const PmergeNode *> pend_set;
  for (std::vector<PmergeNode *>::const_iterator it = pend.begin() + 1;
       it != pend.end(); ++it) {
    pend_set.insert(*it);
    std::cout << (*it)->bignum_ << " ";
  }
  std::cout << ": pend set\n";

  size_t cur_index = 1; // 要素挿入のために進んだ一番右端のindex
  size_t jacob_i = 0;
  size_t cnt_inserted = 0;
  // pendをすべてmainに挿入する
  while (cnt_inserted < pend_set.size()) {
    cur_index =
        std::min(cur_index + jacob_stahal_seq[jacob_i], v_sorted_.size() - 1);
    size_t jacob_cnt =
        jacob_stahal_seq[jacob_i]; // countが0になるまで要素を挿入
    ++jacob_i;
    size_t reverse_i = cur_index; // 挿入するnodeのindex
    while (jacob_cnt != 0) {
      if (pend_set.find(v_sorted_[reverse_i]) == pend_set.end()) {
        --reverse_i;
        continue;
      }
      std::cout << "big num: " << v_sorted_[reverse_i]->bignum_ << '\n';
      // pendのpairを挿入
      PmergeNode *inserting_node = v_sorted_[reverse_i]->pop();
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
  std::cout << "pend inserted\n";
  if (v.size() % 2 != 0) {
    std::cout << v[v.size() - 1]->bignum_ << " inserting: the rest of pair\n";
    // vのあまりを挿入
    BinarySearchInsertion(0, v_sorted_.size() - 1, v[v.size() - 1]);
  }
}

void PmergeMe::BinarySearchInsertion(ssize_t start, ssize_t end,
                                     PmergeNode *key) {
  while (start <= end) {
    size_t middle = (start + end) / 2;
    if (*key < *v_sorted_[middle]) {
      end = middle - 1;
    } else {
      start = middle + 1;
    }
  }
  std::cerr << "inserting " << key->bignum_ << " at index " << start << '\n';
  v_sorted_.insert(v_sorted_.begin() + start, key);
}
