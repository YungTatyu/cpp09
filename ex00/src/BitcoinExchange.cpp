#include "BitcoinExchange.hpp"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

const char *BitcoinExchange::kDefaultRateDataFile_ = "db/data.csv";

BitcoinExchange::BitcoinExchange() { rate_data_file_ = kDefaultRateDataFile_; }

BitcoinExchange::BitcoinExchange(const std::string &file) {
  rate_data_file_ = file;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::ProcessFileForCalculation(const std::string &file) {
  if (!ReadFile(file, price_list_)) {
    return false;
  }
  if (!ParseRateData()) {
    return false;
  }
  bool suc = true;
  for (std::list<std::string>::iterator it = price_list_.begin();
       it != price_list_.end(); ++it) {
    if (it == price_list_.begin()) {
      std::list<std::string> list = Split(*it);
      if (list.size() != 3) {
        PrintError("invalid column name: " + *it);
        return false;
      }
      std::string date = list.front();
      list.pop_front();
      if (date != "date" || list.front() != "|" || list.back() != "value") {
        PrintError("invalid column name: " + *it);
        return false;
      }
      continue;
    }
    bool re = ProcessLineForCalculation(*it);
    suc = (!suc || !re) ? false : true;
  }
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

bool BitcoinExchange::IsValidDate(int year, int month, int date) const {
  if (year < 0 || (month < 1 || month > 12) || date > 31) {
    return false;
  }
  if (month == 2) {
    // うるう年
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
      return date <= 29;
    return date <= 28;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    return date <= 30;
  }
  return true;
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
  int day = Convert<int>(list.back());
  if (!IsValidDate(year, month, day)) {
    throw std::runtime_error(err);
  }
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
    throw std::runtime_error("price is out of range => " + price);
  }
  return d;
}

bool BitcoinExchange::ParseRateData() {
  if (!ReadFile(rate_data_file_, rate_list_)) {
    return false;
  }
  for (std::list<std::string>::iterator it = rate_list_.begin();
       it != rate_list_.end(); ++it) {
    std::list<std::string> list = Split(*it, ",");
    if (it == rate_list_.begin()) {
      if (list.size() != 2) {
        PrintError("invalid column name: " + *it);
        return false;
      }
      if (list.front() != "date" || list.back() != "exchange_rate") {
        PrintError("invalid colums: " + *it);
        return false;
      }
      continue;
    }
    try {
      Date date = ParseDate(list.front());
      double rate = Convert<double>(list.back());
      if (rate < 0.0) {
        throw std::runtime_error("rate cannot be a negative number => " +
                                 list.back());
      }
      if (rate_map_.find(date) != rate_map_.end()) {
        throw std::runtime_error("date cannot be duplicated => " +
                                 list.front());
      }
      rate_map_.insert(std::make_pair(date, rate));
    } catch (const std::exception &e) {
      PrintError(e.what());
      return false;
    }
  }
  if (rate_map_.size() == 0) {
    PrintError("there is no data");
    return false;
  }
  return true;
}

/**
 * rateが見つからない場合は、過去直近のdataを参照する
 */
double BitcoinExchange::FindClosetRate(const Date &date) const {
  std::map<Date, double>::const_iterator it = rate_map_.find(date);
  if (it != rate_map_.end()) {
    return it->second;
  }
  it = rate_map_.lower_bound(date);
  if (it == rate_map_.begin()) {
    throw std::runtime_error("there is no available past rate");
  }
  --it;
  return it->second;
}

bool BitcoinExchange::ProcessLineForCalculation(const std::string &line) {
  if (rate_map_.empty() && !ParseRateData()) {
    return false;
  }
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
    double price = ParsePrice(list.back());
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

bool BitcoinExchange::ReadFile(const std::string &file,
                               std::list<std::string> &list) {
  std::ifstream input_file(file.c_str());
  if (input_file.fail()) {
    PrintError("could not open a file: " + file);
    return false;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    list.push_back(line);
  }
  input_file.close();
  return true;
}

std::ostream &operator<<(std::ostream &os, const Date &date) {
  os << date.year_ << '-' << std::setw(2) << std::setfill('0') << date.month_
     << '-' << std::setw(2) << std::setfill('0') << date.day_;
  return os;
}
