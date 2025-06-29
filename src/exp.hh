#pragma once

#include <cassert>
#include <list>
#include <string>

#include "typedef.hh"

struct exp_t {
  virtual ~exp_t() {}
  virtual auto to_string() const -> std::string = 0;
  virtual auto duplicate() const -> uptr<exp_t> = 0;
  virtual auto eval() -> uptr<exp_t> {
    assert(false); // need implementation
  }

  static auto parse(const std::string_view& str) -> uptr<exp_t>;
};

// we only support integer operation for now
struct exp_number_t : public exp_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp_t> override;
  auto eval() -> uptr<exp_t> override;
  double n = 0;
};

struct exp_string_t : public exp_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp_t> override;
  auto eval() -> uptr<exp_t> override;
  std::string str;
};

struct exp_symbol_t : public exp_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp_t> override;
  std::string str;
};

struct exp_list_t : public exp_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp_t> override;
  std::list<sptr<exp_t>> list;
};

struct exp_quoted_t : public exp_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<exp_t> override;
  auto eval() -> uptr<exp_t> override;
  uptr<exp_t> inner_exp;
};
