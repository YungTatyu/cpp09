#include "PmergeMe.hpp"
#include <algorithm>
#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/time.h>
#include <vector>

size_t PmergeNode::cnt_compare = 0;

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
    if (ss.fail() || !ss.eof() || n < 0) {
      throw std::runtime_error(std::string("error: invalid input ") + *it);
    }
    nums_.push_back(n);
  }
}

std::vector<int>
PmergeMe::MergeInsertionSortVec(const std::list<int> *nums = NULL) {
  if (nums != NULL) {
    nums_ = *nums;
  }
  std::vector<PmergeNode *> v;
  v.reserve(nums_.size());
  if (nums_.size() == 1) {
    return std::vector<int>(nums_.begin(), nums_.end());
  }
  for (std::list<int>::const_iterator it = nums_.begin(); it != nums_.end();
       ++it) {
    v.push_back(new PmergeNode(*it));
  }
  vec_main_.reserve(nums_.size());
  PmergeNode::cnt_compare = 0;
  RecurMergeInsertionSort(v);
  std::vector<int> re;
  re.reserve(nums_.size());
  for (size_t i = 0; i < vec_main_.size(); ++i) {
    re.push_back(vec_main_[i]->bignum_);
    delete vec_main_[i];
  }
  return re;
}

void PmergeMe::RecurMergeInsertionSort(std::vector<PmergeNode *> &v) {
  if (v.size() < 2) {
    PmergeNode *p = v.front();
    vec_main_.push_back(p->pop());
    vec_main_.push_back(p);
    return;
  }
  std::vector<PmergeNode *> paired_v;
  paired_v.reserve(v.size() / 2);
  for (size_t i = 0; i + 1 < v.size(); i += 2) {
    if (*v[i] > *v[i + 1]) {
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
    BinarySearchInsertionVec(0, static_cast<ssize_t>(vec_main_.size() - 1),
                             v[v.size() - 1]);
  }
  if (vec_main_.size() == nums_.size()) {
    return;
  }

  // mainに挿入したいnodeのペアを管理
  std::set<const PmergeNode *> pend;
  // 一番低い数字は飛ばす
  for (std::vector<PmergeNode *>::const_iterator it = vec_main_.begin() + 1;
       it != vec_main_.end(); ++it) {
    pend.insert(*it);
  }
  // 一番低い数字は左端で確定なのではじめに挿入
  vec_main_.insert(vec_main_.begin(), vec_main_.front()->pop());

  size_t cur_index = 1; // 要素挿入のために進んだ一番右端のindex
  size_t jacob_i = 0;
  size_t cnt_inserted = 0;
  // pendのペアをすべてmainに挿入する
  while (cnt_inserted < pend.size()) {
    cur_index =
        std::min(cur_index + jacob_stahal_seq[jacob_i], vec_main_.size() - 1);
    size_t jacob_cnt =
        jacob_stahal_seq[jacob_i]; // countが0になるまで要素を挿入
    ++jacob_i;
    size_t reverse_i = cur_index; // 挿入するnodeのindex
    while (jacob_cnt != 0) {
      if (pend.find(vec_main_[reverse_i]) == pend.end()) {
        --reverse_i;
        continue;
      }
      // pendのpairを挿入
      PmergeNode *inserting_node = vec_main_[reverse_i]->pop();
      BinarySearchInsertionVec(0, static_cast<ssize_t>(reverse_i - 1),
                               inserting_node);
      pend.erase(inserting_node);
      // 挿入するためindexが右にずれる
      ++cur_index;
      --jacob_cnt;
      ++cnt_inserted;
      if (cnt_inserted >= pend.size()) {
        break;
      }
    }
  }
}

/**
 * @brief std::vector に対して二分探索を行い、指定された key
 * を適切な位置に挿入する。
 *
 * この関数ではインデックスとして ssize_t を使用しています。
 * 通常、コンテナのサイズやインデックスには size_t を使うべきですが、
 * 二分探索の過程で end = middle - 1 により -1 になる可能性があるため、
 * 負の値も扱える ssize_t を使用しています。
 *
 * WARN:
 * 特に 64bit 環境では size_t の最大値（2^64 - 1）を ssize_t（2^63 -
 * 1）では表現しきれません。 このため、size_t から ssize_t
 * への変換時にオーバーフローが発生する可能性があります。
 * 本関数はインデックス（およびコンテナのサイズ）が `ssize_t` の最大値を超える
 * ような大規模データには対応していません。
 *
 * @param start 探索開始インデックス（ssize_t）
 * @param end 探索終了インデックス（ssize_t）
 * @param key 挿入すべき要素（PmergeNode*）
 */
void PmergeMe::BinarySearchInsertionVec(ssize_t start, ssize_t end,
                                        PmergeNode *key) {
  while (start <= end) {
    size_t middle = static_cast<size_t>(start + (end - start) / 2);
    if (*key < *vec_main_[middle]) {
      end = static_cast<ssize_t>(middle) - 1;
    } else {
      start = static_cast<ssize_t>(middle) + 1;
    }
  }
  vec_main_.insert(vec_main_.begin() + start, key);
}

std::deque<int>
PmergeMe::MergeInsertionSortDq(const std::list<int> *nums = NULL) {
  if (nums != NULL) {
    nums_ = *nums;
  }
  std::deque<PmergeNode *> q;
  if (nums_.size() == 1) {
    return std::deque<int>(nums_.begin(), nums_.end());
  }
  for (std::list<int>::const_iterator it = nums_.begin(); it != nums_.end();
       ++it) {
    q.push_back(new PmergeNode(*it));
  }
  PmergeNode::cnt_compare = 0;
  RecurMergeInsertionSort(q);
  std::deque<int> re;
  for (size_t i = 0; i < dq_main_.size(); ++i) {
    re.push_back(dq_main_[i]->bignum_);
    delete dq_main_[i];
  }
  return re;
}

void PmergeMe::RecurMergeInsertionSort(std::deque<PmergeNode *> &q) {
  if (q.size() < 2) {
    PmergeNode *p = q.front();
    dq_main_.push_back(p->pop());
    dq_main_.push_back(p);
    return;
  }
  std::deque<PmergeNode *> paired_q;
  for (size_t i = 0; i + 1 < q.size(); i += 2) {
    if (*q[i] > *q[i + 1]) {
      q[i]->push(q[i + 1]);
      paired_q.push_back(q[i]);
    } else {
      q[i + 1]->push(q[i]);
      paired_q.push_back(q[i + 1]);
    }
  }
  RecurMergeInsertionSort(paired_q);
  if (q.size() % 2 != 0) {
    // あまりを挿入
    BinarySearchInsertionDq(0, static_cast<ssize_t>(dq_main_.size() - 1),
                            q[q.size() - 1]);
  }
  if (dq_main_.size() == nums_.size()) {
    return;
  }

  // mainに挿入したいnodeのペアを管理
  std::set<const PmergeNode *> pend;
  // 一番低い数字は飛ばす
  for (std::deque<PmergeNode *>::const_iterator it = dq_main_.begin() + 1;
       it != dq_main_.end(); ++it) {
    pend.insert(*it);
  }
  // 一番低い数字は左端で確定なのではじめに挿入
  dq_main_.push_front(dq_main_.front()->pop());

  size_t cur_index = 1; // 要素挿入のために進んだ一番右端のindex
  size_t jacob_i = 0;
  size_t cnt_inserted = 0;
  // pendのペアをすべてmainに挿入する
  while (cnt_inserted < pend.size()) {
    cur_index =
        std::min(cur_index + jacob_stahal_seq[jacob_i], dq_main_.size() - 1);
    size_t jacob_cnt =
        jacob_stahal_seq[jacob_i]; // countが0になるまで要素を挿入
    ++jacob_i;
    size_t reverse_i = cur_index; // 挿入するnodeのindex
    while (jacob_cnt != 0) {
      if (pend.find(dq_main_[reverse_i]) == pend.end()) {
        --reverse_i;
        continue;
      }
      // pendのpairを挿入
      PmergeNode *inserting_node = dq_main_[reverse_i]->pop();
      BinarySearchInsertionDq(0, static_cast<ssize_t>(reverse_i - 1),
                              inserting_node);
      pend.erase(inserting_node);
      // 挿入するためindexが右にずれる
      ++cur_index;
      --jacob_cnt;
      ++cnt_inserted;
      if (cnt_inserted >= pend.size()) {
        break;
      }
    }
  }
}

/**
 * @brief std::deque に対して二分探索を行い、指定された key
 * を適切な位置に挿入する。
 *
 * この関数ではインデックスとして ssize_t を使用しています。
 * 通常、コンテナのサイズやインデックスには size_t を使うべきですが、
 * 二分探索の過程で end = middle - 1 により -1 になる可能性があるため、
 * 負の値も扱える ssize_t を使用しています。
 *
 * WARN:
 * 特に 64bit 環境では size_t の最大値（2^64 - 1）を ssize_t（2^63 -
 * 1）では表現しきれません。 このため、size_t から ssize_t
 * への変換時にオーバーフローが発生する可能性があります。
 * 本関数はインデックス（およびコンテナのサイズ）が `ssize_t` の最大値を超える
 * ような大規模データには対応していません。
 *
 * @param start 探索開始インデックス（ssize_t）
 * @param end 探索終了インデックス（ssize_t）
 * @param key 挿入すべき要素（PmergeNode*）
 */
void PmergeMe::BinarySearchInsertionDq(ssize_t start, ssize_t end,
                                       PmergeNode *key) {
  while (start <= end) {
    size_t middle = static_cast<size_t>(start + (end - start) / 2);
    if (*key < *dq_main_[middle]) {
      end = static_cast<ssize_t>(middle) - 1;
    } else if (*key > *dq_main_[middle]) {
      start = static_cast<ssize_t>(middle) + 1;
    } else {
      ++start;
      break;
    }
  }
  dq_main_.insert(dq_main_.begin() + start, key);
}
void PmergeMe::SortAndPrint() {
  timeval start, end;
  std::cout << "Before: ";
  for (std::list<int>::const_iterator it = nums_.begin(); it != nums_.end();
       ++it) {
    std::cout << *it << ' ';
  }
  std::cout << '\n';
  gettimeofday(&start, NULL);
  std::vector<int> v = MergeInsertionSortVec();
  gettimeofday(&end, NULL);
  std::cout << "After : ";
  for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << '\n';
  double elapsed_us = CalcElapsedus(start, end);
  std::cout << "Time to process a range of " << v.size()
            << " elements with std::vector : " << elapsed_us << " us\n";
  size_t cnt_vector = PmergeNode::cnt_compare;

  gettimeofday(&start, NULL);
  std::deque<int> q = MergeInsertionSortDq();
  gettimeofday(&end, NULL);
  elapsed_us = CalcElapsedus(start, end);
  std::cout << "Time to process a range of " << v.size()
            << " elements with std::deque  : " << elapsed_us << " us\n";

  std::cout << "Compare counts to process a range of " << v.size()
            << " elements with std::vector : " << cnt_vector << "\n";
  std::cout << "Compare counts to process a range of " << q.size()
            << " elements with std::deque  : " << PmergeNode::cnt_compare
            << "\n";
}

double PmergeMe::CalcElapsedus(const timeval &start, const timeval &end) {
  long seconds = end.tv_sec - start.tv_sec;
  long micros = end.tv_usec - start.tv_usec;
  return static_cast<double>(seconds) * 1e6 + static_cast<double>(micros);
}
