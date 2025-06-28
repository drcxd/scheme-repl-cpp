#pragma once

#include <cassert>
#include <list>
#include <string>

#include "typedef.hh"

struct exp {
  virtual ~exp() {}
  virtual auto to_string() const -> std::string = 0;
  virtual auto duplicate() const -> uptr<exp> = 0;
  virtual auto eval() -> uptr<exp> {
    assert(false); // need implementation
  }

  static auto parse(const std::string_view& str) -> uptr<exp>;
};

// we only support integer operation for now
struct exp_number : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp> override;
  auto eval() -> uptr<exp> override;
  double n = 0;
};

struct exp_string : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp> override;
  auto eval() -> uptr<exp> override;
  std::string str;
};

struct exp_symbol : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp> override;
  std::string str;
};

// quoted values are supported as lists: (quote ...)
struct exp_list : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp> override;
  std::list<uptr<exp>> list;
};

struct exp_quoted : public exp {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp> override;
  auto eval() -> uptr<exp> override;
  uptr<exp> inner_exp;
};
