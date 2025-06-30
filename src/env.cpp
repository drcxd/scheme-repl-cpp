#include "env.hh"

#include <cassert>

#include "exp.hh"

auto env_t::lookup_variable_value(const std::string_view &var) -> exp_t * {
  for (auto &frame : frames) {
    for (auto &record : frame->records) {
      if (record->first == var) {
        return record->second.get();
      }
    }
  }
  return nullptr;
}

auto env_t::define_variable(const std::string_view &var, uptr<exp_t> value)
    -> void {
  assert(!frames.empty());
  auto &first_frame = frames.front();
  bool update = false;
  for (auto &record : first_frame->records) {
    if (record->first == var) {
      record->second = std::move(value);
      update = true;
    }
  }
  if (!update) {
    first_frame->records.push_back(
        std::make_shared<record_t>(std::make_pair(var, std::move(value))));
  }
}

namespace genv {
env_t global;
auto init_global_environment() -> void {
  // an empty frame with nothing
  global.frames.push_front(std::make_shared<frame_t>());
}

auto get_global_environment() -> env_t * { return &global; }
} // namespace genv
