#pragma once

#include <cassert>
#include <list>
#include <memory>
#include <string>

struct exp {
  virtual ~exp() {}
  virtual auto to_string() const -> std::string = 0;
  virtual auto duplicate() const -> std::unique_ptr<exp> = 0;
  virtual auto eval() -> std::unique_ptr<exp> {
    assert(false); // need implementation
  }

  static auto parse(const std::string_view& str) -> std::unique_ptr<exp>;
};

// we only support integer operation for now
struct exp_number : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> std::unique_ptr<exp> override;
  auto eval() -> std::unique_ptr<exp> override;
  double n = 0;
};

struct exp_string : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> std::unique_ptr<exp> override;
  auto eval() -> std::unique_ptr<exp> override;
  std::string str;
};

struct exp_symbol : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> std::unique_ptr<exp> override;
  std::string str;
};

// quoted values are supported as lists: (quote ...)
struct exp_list : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> std::unique_ptr<exp> override;
  std::list<std::unique_ptr<exp>> list;
};

struct exp_quoted : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> std::unique_ptr<exp> override;
  std::unique_ptr<exp> inner_exp;
};
