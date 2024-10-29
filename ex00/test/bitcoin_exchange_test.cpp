#include "gtest/gtest-spi.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <limits>
#include <stdexcept>

#include "BitcoinExchange.hpp"

namespace test {
RPN Setup(const std::string &line, long min, long max) {
  RPN rpn(line);
  rpn.SetMin(min);
  rpn.SetMax(max);
  return rpn;
}

} // namespace test

TEST(bitcoin_exchange_test, calc_line1) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2012-01-11|0"));
}

TEST(bitcoin_exchange_test, calc_line2) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("  2022-01-11   |    1000  "));
}

TEST(bitcoin_exchange_test, calc_line3) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2023-01-31 | 999.9999"));
}

TEST(bitcoin_exchange_test, calc_line4) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2023-12-31 | 42"));
}

TEST(bitcoin_exchange_test, calc_line5) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2023-1-10 | 42"));
}

TEST(bitcoin_exchange_test, calc_line6) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2023-1-01 | 42"));
}

TEST(bitcoin_exchange_test, calc_line7) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2023-1-01 | -0"));
}

TEST(bitcoin_exchange_test, calc_line8) {
  BitcoinExchange be("db/data.csv");
  EXPECT_TRUE(be.ProcessLineForCalculation("2023-1-01 | +0"));
}

TEST(bitcoin_exchange_test, error_empty1) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(""));
}

TEST(bitcoin_exchange_test, error_empty2) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation("    "));
}

TEST(bitcoin_exchange_test, error_invalid_date1) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2023-02-31 | 2   "));
}

TEST(bitcoin_exchange_test, error_invalid_date2) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2023-13-1 | 2   "));
}

TEST(bitcoin_exchange_test, error_invalid_date3) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" -42-1-1 | 2   "));
}

TEST(bitcoin_exchange_test, error_invalid_price1) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 1000.1   "));
}

TEST(bitcoin_exchange_test, error_invalid_price2) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | -1   "));
}

TEST(bitcoin_exchange_test, error_invalid_price3) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | ++1   "));
}

TEST(bitcoin_exchange_test, error_invalid_price4) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | --1   "));
}

TEST(bitcoin_exchange_test, error_invalid_price5) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 1test   "));
}

TEST(bitcoin_exchange_test, error_invalid_price6) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 1.0t"));
}

TEST(bitcoin_exchange_test, error_invalid_price7) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 1000."));
}

TEST(bitcoin_exchange_test, error_invalid_input1) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" test   "));
}

TEST(bitcoin_exchange_test, error_invalid_input2) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | test   "));
}

TEST(bitcoin_exchange_test, error_invalid_input3) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 || 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_input4) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   |"));
}

TEST(bitcoin_exchange_test, error_invalid_input5) {
  BitcoinExchange be("db/data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation("| 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv1) {
  BitcoinExchange be("dont_exist.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv2) {
  BitcoinExchange be("db/no_data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv3) {
  BitcoinExchange be("db/duplicate_data.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv4) {
  BitcoinExchange be("db/invalid_date1.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv5) {
  BitcoinExchange be("db/invalid_date2.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv6) {
  BitcoinExchange be("db/invalid_date3.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv7) {
  BitcoinExchange be("db/invalid_date4.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv8) {
  BitcoinExchange be("db/invalid_date5.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv9) {
  BitcoinExchange be("db/invalid_rate1.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv10) {
  BitcoinExchange be("db/invalid_rate2.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv11) {
  BitcoinExchange be("db/invalid_rate3.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}

TEST(bitcoin_exchange_test, error_invalid_csv12) {
  BitcoinExchange be("db/invalid_rate4.csv");
  EXPECT_FALSE(be.ProcessLineForCalculation(" 2010-1-1 | 42   "));
}
