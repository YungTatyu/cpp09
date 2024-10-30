#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <cstdio>
#include <iomanip>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <utility>

struct Date {
  Date(int year, int month, int day) {
    year_ = year;
    month_ = month;
    day_ = day;
  }
  Date(const Date &other) { *this = other; }
  ~Date() {}
  Date &operator=(const Date &other) {
    if (this != &other) {
      year_ = other.year_;
      month_ = other.month_;
      day_ = other.day_;
    }
    return *this;
  }
  bool operator<(const Date &other) const {
    if (year_ != other.year_) {
      return year_ < other.year_;
    }
    if (month_ != other.month_) {
      return month_ < other.month_;
    }
    return day_ < other.day_;
  }

  int year_;
  int month_;
  int day_;

private:
  Date();
};

std::ostream &operator<<(std::ostream &os, const Date &date);

class BitcoinExchange {
public:
  BitcoinExchange();
  BitcoinExchange(const std::string &);
  ~BitcoinExchange();
  bool ProcessFileForCalculation(const std::string &);
  bool ProcessLineForCalculation(const std::string &);

  std::string rate_data_file_;
  std::list<std::string> rate_list_;
  std::map<Date, double> rate_map_;
  std::list<std::string> price_list_;

private:
  BitcoinExchange(const BitcoinExchange &);
  BitcoinExchange &operator=(const BitcoinExchange &);
  bool ParseRateData();
  std::pair<Date, double> ParsePriceLine(const std::string &);
  Date ParseDate(const std::string &) const;
  double ParsePrice(const std::string &) const;
  double FindClosetRate(const Date &) const;
  bool ReadFile(const std::string &, std::list<std::string> &);
  bool IsPriceInRange(const std::string &) const;
  bool IsValidDate(int year, int month, int date) const;
  std::list<std::string> Split(const std::string &,
                               const std::string & = " \f\n\r\t\v") const;
  void PrintError(const std::string &) const;
  template <typename T> T Convert(const std::string &) const;

  static const char *kDefaultRateDataFile_;
  static const int kMinPrice_ = 0;
  static const int kMaxPrice_ = 1000;
};

template <typename T> T BitcoinExchange::Convert(const std::string &str) const {
  std::istringstream iss(str);
  T result;
  iss >> result;
  if (iss.fail() || iss.peek() != EOF) {
    throw std::runtime_error("bad input => " + str);
  }
  return result;
}

#endif // !BITCOIN_EXCHANGE_HPP
