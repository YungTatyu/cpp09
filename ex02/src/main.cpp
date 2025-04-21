#include "PmergeMe.hpp"
#include <iostream>
#include <list>

void usage(const std::string &str) { std::cerr << "usage: " << str << "\n"; }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    usage(std::string(argv[0]) + " numbers");
    return 1;
  }
  try {
    std::list<std::string> list(argv + 1, argv + argc);
    PmergeMe p(list);
    p.SortAndPrint();
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
