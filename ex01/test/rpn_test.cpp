#include "gtest/gtest-spi.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <limits>
#include <stdexcept>

#include "RPN.hpp"

namespace test {
RPN Setup(const std::string &line, long min, long max) {
  RPN rpn(line);
  rpn.SetMin(min);
  rpn.SetMax(max);
  return rpn;
}

} // namespace test

TEST(rpn_test, simple_exp1) {
  RPN rpn("1 9 + 5  - 2  / 3 *");
  EXPECT_EQ(rpn.Calculate(), 6);
}

TEST(rpn_test, simple_exp2) {
  RPN rpn("3   5 +  ");
  EXPECT_EQ(rpn.Calculate(), 8);
}

TEST(rpn_test, simple_exp3) {
  RPN rpn("10   2 /   ");
  EXPECT_EQ(rpn.Calculate(), 5);
}

TEST(rpn_test, simple_exp4) {
  RPN rpn("7  8  *  3 - ");
  EXPECT_EQ(rpn.Calculate(), 53);
}

TEST(rpn_test, simple_exp5) {
  RPN rpn("9  3 /  5  *  ");
  EXPECT_EQ(rpn.Calculate(), 15);
}

TEST(rpn_test, simple_exp6) {
  RPN rpn("    12 4 / 2 * ");
  EXPECT_EQ(rpn.Calculate(), 6);
}

TEST(rpn_test, simple_exp7) {
  RPN rpn("8 3 -   4 + 2 * ");
  EXPECT_EQ(rpn.Calculate(), 18);
}

TEST(rpn_test, simple_exp8) {
  RPN rpn(" 6 3  *  2 /  ");
  EXPECT_EQ(rpn.Calculate(), 9);
}

TEST(rpn_test, simple_exp9) {
  RPN rpn("10 2 - 4  *  ");
  EXPECT_EQ(rpn.Calculate(), 32);
}

TEST(rpn_test, simple_exp10) {
  RPN rpn("5 1 2 +  4 *  + 3 -");
  EXPECT_EQ(rpn.Calculate(), 14);
}

TEST(rpn_test, error_no_operator1) {
  RPN rpn("5 1 ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_no_operator2) {
  RPN rpn(" 1 ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_empty1) {
  RPN rpn("");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_empty2) {
  RPN rpn("   ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_no_number1) {
  RPN rpn("+  ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_no_number2) {
  RPN rpn(" /   +  ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_divide_by_zero) {
  RPN rpn("10 0 /");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_overflow1) {
  RPN rpn =
      test::Setup("9223372036854775807 1 +", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_overflow2) {
  RPN rpn =
      test::Setup("9223372036854775807 10 *", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_underflow1) {
  RPN rpn =
      test::Setup("-9223372036854775808 1 -", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_underflow2) {
  RPN rpn =
      test::Setup("-9223372036854775808 10 *", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}
