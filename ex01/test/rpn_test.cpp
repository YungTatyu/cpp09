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

TEST(rpn_test, exp1) {
  RPN rpn("1 9 + 5  - 2  / 3 *");
  EXPECT_EQ(rpn.Calculate(), 6);
}

TEST(rpn_test, exp2) {
  RPN rpn("3   5 +  ");
  EXPECT_EQ(rpn.Calculate(), 8);
}

TEST(rpn_test, exp3) {
  RPN rpn("9   2 /   ");
  EXPECT_EQ(rpn.Calculate(), 4);
}

TEST(rpn_test, exp4) {
  RPN rpn("7  8  *  3 - ");
  EXPECT_EQ(rpn.Calculate(), 53);
}

TEST(rpn_test, exp5) {
  RPN rpn("9  3 /  5  *  ");
  EXPECT_EQ(rpn.Calculate(), 15);
}

TEST(rpn_test, exp6) {
  RPN rpn("    1 4 / 2 * ");
  EXPECT_EQ(rpn.Calculate(), 0);
}

TEST(rpn_test, exp7) {
  RPN rpn("8 3 -   4 + 2 * ");
  EXPECT_EQ(rpn.Calculate(), 18);
}

TEST(rpn_test, exp8) {
  RPN rpn(" 6 3  *  2 /  ");
  EXPECT_EQ(rpn.Calculate(), 9);
}

TEST(rpn_test, exp9) {
  RPN rpn("9 2 - 4  *  ");
  EXPECT_EQ(rpn.Calculate(), 28);
}

TEST(rpn_test, exp10) {
  RPN rpn("5 1 2 +  4 *  + 3 -");
  EXPECT_EQ(rpn.Calculate(), 14);
}

TEST(rpn_test, exp11) {
  RPN rpn = test::Setup("-1-2+", std::numeric_limits<long>::min(),
                        std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), -3);
}

TEST(rpn_test, exp12) {
  RPN rpn = test::Setup("-1-1--1-", std::numeric_limits<long>::min(),
                        std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), 1);
}

TEST(rpn_test, exp13) {
  RPN rpn = test::Setup("+1+2++10+", std::numeric_limits<long>::min(),
                        std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), 13);
}

TEST(rpn_test, exp14) {
  RPN rpn = test::Setup("1 10 100 1000 10000 1 / * + -+",
                        std::numeric_limits<long>::min(),
                        std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), -10000089);
}

TEST(rpn_test, lerge_exp1) {
  RPN rpn =
      test::Setup("-9223372036854775808 1 *", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), -9223372036854775808L);
}

TEST(rpn_test, lerge_exp2) {
  RPN rpn =
      test::Setup("-9223372036854775808 0 +", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), -9223372036854775808L);
}

TEST(rpn_test, lerge_exp3) {
  RPN rpn =
      test::Setup("9223372036854775806 1 +", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_EQ(rpn.Calculate(), 9223372036854775807L);
}

TEST(rpn_test, only_nomber1) {
  RPN rpn("     4      ");
  EXPECT_EQ(rpn.Calculate(), 4);
}

TEST(rpn_test, only_nomber2) {
  RPN rpn("0");
  EXPECT_EQ(rpn.Calculate(), 0);
}

TEST(rpn_test, error_no_operator1) {
  RPN rpn("5 1 ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}
TEST(rpn_test, error_no_operator2) {
  RPN rpn(" 1    4     10 ");
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

TEST(rpn_test, error_str1) {
  RPN rpn("-1-1-- ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_str2) {
  RPN rpn("-1-1-- ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_str3) {
  RPN rpn("-1-1-str -1 ");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_str4) {
  RPN rpn("1test1");
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_str5) {
  RPN rpn("1 1 + 2 3 - -      s ");
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

TEST(rpn_test, error_overflow3) {
  RPN rpn =
      test::Setup("-9223372036854775808 -1 *", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}

TEST(rpn_test, error_overflow4) {
  RPN rpn = test::Setup("-1 -9223372036854775808  *",
                        std::numeric_limits<long>::min(),
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

TEST(rpn_test, error_underflow3) {
  RPN rpn =
      test::Setup("92233720368547758 -1000 *", std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
  EXPECT_THROW(rpn.Calculate();, std::runtime_error);
}
