#include "PmergeMe.hpp"
#include <iostream>
#include <list>

void usage(const std::string &str) { std::cerr << "usage: " << str << "\n"; }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << argv[0] << " numbers\n";
    usage(std::string(argv[0]) + " numbers");
    return 1;
  }
  try {
    std::list<std::string> list(argv + 1, argv + argc);
    PmergeMe p(list);
    p.MergeInsertionSortV();
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    usage(std::string(argv[0]) + " numbers");
    return 1;
  }
  return 0;
}
