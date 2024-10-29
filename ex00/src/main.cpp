
#include <iostream>

#include "BitcoinExchange.hpp"
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usuage: " << argv[0] << "<database>";
    return 1;
  }
  BitcoinExchange bc(argv[1]);

  return 0;
}
