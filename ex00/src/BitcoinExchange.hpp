#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <map>
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

class BitcoinExchange {
public:
  BitcoinExchange();
  BitcoinExchange(const std::string &);
  BitcoinExchange(const BitcoinExchange &);
  ~BitcoinExchange();
  BitcoinExchange &operator=(const BitcoinExchange &);
  bool ProcessFileForCalculation(const std::string &);
  bool ProcessLineForCalculation(const std::string &);

  std::string raw_rate_data_;
  std::string raw_price_data_; // use this when parsing price file
  std::map<Date, double> rate_map_;

private:
  bool ParseRateData();
  std::pair<Date, double> ParsePriceLine(const std::string &);
  Date ParseDate(const std::string &) const;
  double ParsePrice(const std::string &) const;
  double FindClosetRate(const Date &) const;
  double CalcValue(const Date &) const;
  std::string ReadFile(const std::string &) const;

  static const char *kDefaultRateDataFile_;
  static const int kMinPrice_ = 0;
  static const int kMaxPrice_ = 1000;
};

#endif // !BITCOIN_EXCHANGE_HPP
