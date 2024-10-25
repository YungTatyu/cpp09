#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <string>

class BitcoinExchange {
public:
  BitcoinExchange(const std::string &);
  ~BitcoinExchange();

  std::string rate_database_;
  std::string price_database_;

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

  static const char *rate_database_file_ = "";
  static const int kMinPrice_ = 0;
  static const int kMaxPrice_ = 1000;
};

#endif // !BITCOIN_EXCHANGE_HPP
