
#include "RPN.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usuage: " << argv[0] << " <expression>\n";
    return 1;
  }
  RPN rpn(argv[1]);
  return 0;
}
