#include <iostream>

#include "exp.hh"

int main(int argc, char **argv) {
  std::string input;
  while (true) {
    std::cout << "Input:\n";
    std::getline(std::cin, input);
    auto exp = exp::parse(input);
    std::cout << "Output:\n";
    std::cout << exp->to_string() << "\n";
  }
  return 0;
}
