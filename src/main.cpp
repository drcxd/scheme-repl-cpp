#include <iostream>

#include "env.hh"
#include "exp.hh"

int main(int argc, char **argv) {

  env::init_global_environment();

  std::string input;
  while (true) {
    std::cout << "Input:\n";
    std::getline(std::cin, input);
    auto e = exp_t::parse(input);
    auto r = e->eval();
    std::cout << "Output:\n";
    std::cout << r->to_string() << "\n";
  }
  return 0;
}
