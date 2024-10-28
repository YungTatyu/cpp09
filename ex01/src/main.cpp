
#include "RPN.hpp"
#include <exception>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usuage: " << argv[0] << " <expression>\n";
    return 1;
  }
  RPN rpn;
  try {
    std::cout << rpn.Calculate(argv[1]) << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
