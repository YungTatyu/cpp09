
#include <iostream>

#include "BitcoinExchange.hpp"
int main(int argc, char *argv[]) {
  if (argc != 2 && argc != 3) {
    std::cerr << "usuage: " << argv[0]
              << " database for price [database for rate]" << std::endl;
    return 1;
  }
  if (argc == 3) {
    BitcoinExchange bc(argv[2]);
    if (!bc.ProcessFileForCalculation(argv[1])) {
      return 1;
    }
    return 0;
  }
  BitcoinExchange bc;
  if (!bc.ProcessFileForCalculation(argv[1])) {
    return 1;
  }
  return 0;
}
