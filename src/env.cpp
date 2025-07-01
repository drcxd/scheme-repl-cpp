#include "env.hh"

#include <cassert>

#include "exp.hh"

namespace env {
  std::list<sptr<frame_t>> global;
  env_t curenv;

auto init_global_environment() -> void {
  // an empty frame with nothing
  global.push_front(std::make_shared<frame_t>());
  curenv = global.begin();
}

auto get_global_environment() -> env_t { return global.begin(); }

auto get_current_environment() -> env_t { return curenv; }

auto lookup_variable_value(const std::string_view &var, env_t env) -> exp_t * {
  while (env != global.end()) {
    auto &frame = *env;
    for (auto &record : *frame) {
      if (record->first == var) {
        return record->second.get();
      }
    }
    ++env;
  }
  return nullptr;
}

auto define_variable(const std::string_view &var, uptr<exp_t> value, env_t env)
    -> void {
  assert(env != global.end());
  auto &first_frame = *env;
  bool update = false;
  for (auto &record : *first_frame) {
    if (record->first == var) {
      record->second = std::move(value);
      update = true;
    }
  }
  if (!update) {
    first_frame->push_back(
        std::make_shared<record_t>(std::make_pair(var, std::move(value))));
  }
}
} // namespace env
