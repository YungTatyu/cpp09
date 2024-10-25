#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <map>
#include <string>

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

  int year_;
  int month_;
  int day_;

private:
  Date();
};

class BitcoinExchange {
public:
  BitcoinExchange(const std::string &);
  ~BitcoinExchange();

  std::string raw_rate_data_;
  std::string raw_price_data_;
  std::map<Date, double> rate_map_;
  std::map<Date, double> price_map_;

private:
  BitcoinExchange();
  BitcoinExchange(const BitcoinExchange &);
  BitcoinExchange &operator=(const BitcoinExchange &);
  bool parseRateDatabase();
  bool parsePriceDatabase();
  bool isDate(const std::string &) const;
  bool isValidPrice(const std::string &) const;
  double findClosetPrice() const;
  double findClosetRate() const;

  static const char *kRateDatabaseFile_ = "";
  static const int kMinPrice_ = 0;
  static const int kMaxPrice_ = 1000;
};

#endif // !BITCOIN_EXCHANGE_HPP
