#pragma once

#include <list>
#include <string>

#include "typedef.hh"

struct exp;

using record_t = std::pair<std::string, uptr<exp>>;

struct frame_t {
  std::list<sptr<record_t>> records;
};

struct env_t {
  std::list<sptr<frame_t>> frames;

  auto lookup_variable_value(const std::string_view &var) -> exp *;
  auto define_variable(const std::string_view &var, uptr<exp> value) -> void;
};

namespace genv {
  auto init_global_environment() -> void;
  auto get_global_environment() -> env_t;
}
