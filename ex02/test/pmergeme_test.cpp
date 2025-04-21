#include "gtest/gtest-spi.h"
#include <algorithm>
#include <cstddef>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <limits>
#include <list>
#include <random>
#include <vector>

#include "PmergeMe.hpp"

const int DEFAULT_MIN = std::numeric_limits<int>::min();
const int DEFAULT_MAX = std::numeric_limits<int>::max();

namespace test {
std::list<int> GenerateRandomList(size_t size, int min = DEFAULT_MIN,
                                  int max = DEFAULT_MAX) {
  std::list<int> result;

  // 乱数生成器の初期化
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(min, max);

  for (int i = 0; i < size; ++i) {
    result.push_back(dist(gen));
  }

  return result;
}
} // namespace test

TEST(mis_test, vec_one_element1) {
  PmergeMe p;
  std::list<int> list = {1};

  std::vector<int> actual = p.MergeInsertionSortV(&list);
  std::vector<int> expect(list.begin(), list.end());
  std::sort(expect.begin(), expect.end());
  EXPECT_EQ(actual, expect);
}

TEST(mis_test, vec_one_element2) {
  PmergeMe p;
  std::list<int> list = {-100};

  std::vector<int> actual = p.MergeInsertionSortV(&list);
  std::vector<int> expect(list.begin(), list.end());
  std::sort(expect.begin(), expect.end());
  EXPECT_EQ(actual, expect);
}

TEST(mis_test, vec_one_element3) {
  PmergeMe p;
  std::list<int> list = {0};

  std::vector<int> actual = p.MergeInsertionSortV(&list);
  std::vector<int> expect(list.begin(), list.end());
  std::sort(expect.begin(), expect.end());
  EXPECT_EQ(actual, expect);
}

TEST(mis_test, vec_two_elements) {
  for (int i = 0; i < 10; ++i) {
    PmergeMe p;
    std::list<int> list = test::GenerateRandomList(2);

    std::vector<int> actual = p.MergeInsertionSortV(&list);
    std::vector<int> expect(list.begin(), list.end());
    std::sort(expect.begin(), expect.end());
    EXPECT_EQ(actual, expect);
  }
}

TEST(mis_test, vec_three_elements) {
  for (int i = 0; i < 10; ++i) {
    PmergeMe p;
    std::list<int> list = test::GenerateRandomList(3);

    std::vector<int> actual = p.MergeInsertionSortV(&list);
    std::vector<int> expect(list.begin(), list.end());
    std::sort(expect.begin(), expect.end());
    EXPECT_EQ(actual, expect);
  }
}

TEST(mis_test, vec_four_elements) {
  for (int i = 0; i < 10; ++i) {
    PmergeMe p;
    std::list<int> list = test::GenerateRandomList(4);

    std::vector<int> actual = p.MergeInsertionSortV(&list);
    std::vector<int> expect(list.begin(), list.end());
    std::sort(expect.begin(), expect.end());
    EXPECT_EQ(actual, expect);
  }
}

TEST(mis_test, vec_five_elements) {
  for (int i = 0; i < 10; ++i) {
    PmergeMe p;
    std::list<int> list = test::GenerateRandomList(5);

    std::vector<int> actual = p.MergeInsertionSortV(&list);
    std::vector<int> expect(list.begin(), list.end());
    std::sort(expect.begin(), expect.end());
    EXPECT_EQ(actual, expect);
  }
}

TEST(mis_test, vec_threethousands_elements) {
  for (int i = 0; i < 10; ++i) {
    PmergeMe p;
    std::list<int> list = test::GenerateRandomList(3000);

    std::vector<int> actual = p.MergeInsertionSortV(&list);
    std::vector<int> expect(list.begin(), list.end());
    std::sort(expect.begin(), expect.end());
    EXPECT_EQ(actual, expect);
  }
}

TEST(mis_test, vec_tenthousands_elements) {
  for (int i = 0; i < 10; ++i) {
    PmergeMe p;
    std::list<int> list = test::GenerateRandomList(10000);

    std::vector<int> actual = p.MergeInsertionSortV(&list);
    std::vector<int> expect(list.begin(), list.end());
    std::sort(expect.begin(), expect.end());
    EXPECT_EQ(actual, expect);
  }
}
