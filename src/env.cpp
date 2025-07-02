#include "env.hh"

#include <cassert>

#include "exp.hh"
#include "procedure.hh"

namespace env {
std::list<sptr<frame_t>> global;
env_t curenv;

auto init_global_environment() -> void {
  global.clear();
  auto base_frame = std::make_shared<frame_t>();
  base_frame->push_back(
      std::make_shared<record_t>(std::make_pair<std::string, uptr<obj_t>>(
          "+", std::make_unique<proc_add_t>())));
  base_frame->push_back(
      std::make_shared<record_t>(std::make_pair<std::string, uptr<obj_t>>(
          "-", std::make_unique<proc_min_t>())));
  base_frame->push_back(
      std::make_shared<record_t>(std::make_pair<std::string, uptr<obj_t>>(
          "*", std::make_unique<proc_mul_t>())));
  base_frame->push_back(
      std::make_shared<record_t>(std::make_pair<std::string, uptr<obj_t>>(
          "/", std::make_unique<proc_div_t>())));
  global.push_front(base_frame);
  curenv = global;
}

auto get_global_environment() -> env_t { return global; }

auto get_current_environment() -> env_t { return curenv; }

auto lookup_variable_value(const std::string_view &var, env_t env)
    -> obj_t * {
  for (auto &frame : env) {
    for (auto &record : *frame) {
      if (record->first == var) {
        return record->second.get();
      }
    }
  }
  return nullptr;
}

auto define_variable(const std::string_view &var, uptr<obj_t> value,
                     env_t env) -> void {
  assert(!env.empty());
  auto &first_frame = *env.begin();
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
