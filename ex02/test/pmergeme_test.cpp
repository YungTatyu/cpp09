#include "gtest/gtest-spi.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <limits>
#include <list>
#include <vector>

#include "PmergeMe.hpp"

TEST(mis_test, vec_one_element) {
  PmergeMe p;
  std::list<int> list = {1};
  std::vector<int> actual = p.MergeInsertionSortV(&list);

  std::vector<int> expect(list.begin(), list.end());
  std::sort(expect.begin(), expect.end());
  EXPECT_EQ(actual, expect);
}
