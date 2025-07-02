#pragma once

#include <cassert>
#include <list>
#include <string>

#include "typedef.hh"


struct obj_t {
  virtual ~obj_t() = default;
  virtual auto duplicate() const -> uptr<obj_t> = 0;
  virtual auto to_string() const -> std::string = 0;
  virtual auto eval() -> uptr<obj_t> {
    assert(false); // need implementation
  }
};

auto parse_exp(const std::string_view& str) -> uptr<obj_t>;

struct obj_number_t : public obj_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<obj_t> override;
  auto eval() -> uptr<obj_t> override;
  double n = 0;
};

struct obj_string_t : public obj_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<obj_t> override;
  auto eval() -> uptr<obj_t> override;
  std::string str;
};

struct obj_symbol_t : public obj_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<obj_t> override;
  auto eval() -> uptr<obj_t> override;
  std::string str;
};

struct obj_list_t : public obj_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<obj_t> override;
  auto eval() -> uptr<obj_t> override;
  std::list<sptr<obj_t>> list;
};

struct obj_quoted_t : public obj_t {
  auto to_string() const -> std::string override;
  auto duplicate() const -> uptr<obj_t> override;
  auto eval() -> uptr<obj_t> override;
  uptr<obj_t> inner_obj;
};

struct obj_define_t : public obj_list_t {
  auto eval() -> uptr<obj_t> override;
};

struct obj_branch_t : public obj_list_t {
  auto eval() -> uptr<obj_t> override;
};

struct obj_lambda_t : public obj_list_t {
  auto eval() -> uptr<obj_t> override;
  auto duplicate() const -> uptr<obj_t> override;
};
