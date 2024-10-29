#include "BitcoinExchange.hpp"

#include <cctype>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

const char *BitcoinExchange::kDefaultRateDataFile_ = "db/data.csv";

BitcoinExchange::BitcoinExchange() { rate_data_file_ = kDefaultRateDataFile_; }

BitcoinExchange::BitcoinExchange(const std::string &file) {
  rate_data_file_ = file;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::ProcessFileForCalculation(const std::string &file) {
  if (!ReadFile(file.c_str())) {
    PrintError("could not open a file: " + file);
    return false;
  }
  bool suc = true;
  return suc;
}

std::list<std::string>
BitcoinExchange::Split(const std::string &line,
                       const std::string &delimiter) const {
  enum { sw_start, sw_val, sw_delim } state = sw_start;
  std::list<std::string> list;
  std::string cur_val;
  size_t i = 0;
  while (i < line.size()) {
    char ch = line[i];
    switch (state) {
    case sw_start:
      if (delimiter.find(ch) != std::string::npos) {
        state = sw_delim;
        break;
      }
      state = sw_val;
      break;
    case sw_val:
      if (delimiter.find(ch) != std::string::npos) {
        list.push_back(cur_val);
        cur_val.clear();
        state = sw_delim;
        break;
      }
      cur_val += ch;
      ++i;
      break;
    case sw_delim:
      if (delimiter.find(ch) == std::string::npos) {
        state = sw_val;
        break;
      }
      ++i;
    }
  }
  if (!cur_val.empty()) {
    list.push_back(cur_val);
  }
  return list;
}

Date BitcoinExchange::ParseDate(const std::string &date) const {
  std::list<std::string> list = Split(date, "-");
  std::string err = "bad input => " + date;
  // yyyy-mm-dd
  if (list.size() != 3) {
    throw std::runtime_error(err);
  }
  int year = Convert<int>(list.front());
  list.pop_front();
  int month = Convert<int>(list.front());
  list.pop_front();
  int day = Convert<int>(list.front());
  return Date(year, month, day);
}

bool BitcoinExchange::IsPriceInRange(const std::string &str) const {
  std::string::size_type dot_pos = str.find(".");
  std::string integer_part = str.substr(0, dot_pos);
  int integer = Convert<int>(integer_part);

  // 整数部が1000かつ小数点がある場合は、1000を越えていないか確認する
  if (integer == kMaxPrice_ && dot_pos != std::string::npos) {
    std::string decimal_part = str.substr(dot_pos + 1);
    return decimal_part.find_first_of("123456789") == std::string::npos;
  }
  return integer <= kMaxPrice_ && integer >= kMinPrice_;
}

double BitcoinExchange::ParsePrice(const std::string &price) const {
  double d = Convert<double>(price);
  if (!IsPriceInRange(price)) {
    throw std::runtime_error("price is out of range");
  }
  return d;
}

bool BitcoinExchange::ProcessLineForCalculation(const std::string &line) {
  std::list<std::string> list = Split(line);
  if (list.size() != 3) {
    PrintError("bad input => " + line);
    return false;
  }
  try {
    Date date = ParseDate(list.front());
    list.pop_front();
    std::string pipe = list.front();
    if (pipe != "|") {
      PrintError("unexpected input => " + pipe);
      return false;
    }
    list.pop_front();
    double price = ParsePrice(list.front());
    double rate = FindClosetRate(date);
    double value = price * rate;
    std::cout << date << " => " << price << " = " << value << std::endl;
  } catch (const std::exception &e) {
    PrintError(e.what());
    return false;
  }
  return true;
}

void BitcoinExchange::PrintError(const std::string &msg) const {
  std::string errmsg = msg.empty() ? "Error" : "Error: " + msg;
  std::cerr << errmsg << std::endl;
}

bool BitcoinExchange::ReadFile(const std::string &file) {
  std::ifstream input_file(file.c_str());
  if (input_file.fail()) {
    return false;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    rate_stack_.push_back(line);
  }
  input_file.close();
  return true;
}

std::ostream &operator<<(std::ostream &os, const Date &date) {
  os << date.year_ << '-' << std::setw(2) << std::setfill('0') << date.month_
     << '-' << std::setw(2) << std::setfill('0') << date.day_;
  return os;
}
