#include <iostream>

#include "exp.hh"
#include "eval.hh"

int main(int argc, char **argv) {
  std::string input;
  while (true) {
    std::cout << "Input:\n";
    std::getline(std::cin, input);
    auto e = exp::parse(input);
    auto r = eval(e.get());
    std::cout << "Output:\n";
    std::cout << r->to_string() << "\n";
  }
  return 0;
}
