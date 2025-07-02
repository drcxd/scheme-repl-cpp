#pragma once

#include "typedef.hh"

#include "exp.hh"

struct proc_t : public obj_t {
  std::list<sptr<obj_t>> params;
  std::list<sptr<obj_t>> body;
  env_t env;

  virtual auto apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t>;
  auto duplicate() const -> sptr<obj_t> override;
  auto to_string() const -> std::string override;
};

struct proc_add_t : public proc_t {
  auto apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> override;
  auto duplicate() const -> sptr<obj_t> override;
};

struct proc_min_t : public proc_t {
  auto apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> override;
  auto duplicate() const -> sptr<obj_t> override;
};

struct proc_mul_t : public proc_t {
  auto apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> override;
  auto duplicate() const -> sptr<obj_t> override;
};

struct proc_div_t : public proc_t {
  auto apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> override;
  auto duplicate() const -> sptr<obj_t> override;
};
