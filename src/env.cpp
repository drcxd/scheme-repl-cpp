#include "env.hh"
#include <cassert>

auto env_t::lookup_variable_value(const std::string_view &var) -> exp * {
  for (auto &frame : frames) {
    for (auto &record : frame->records) {
      if (record->first == var) {
        return record->second.get();
      }
    }
  }
  return nullptr;
}

auto env_t::define_variable(const std::string_view &var, uptr<exp> value)
    -> void {
  assert(!frames.empty());
  auto &first_frame = frames.front();
  for (auto &record : first_frame->records) {
    if (record->first == var) {
      record->second = std::move(value);
    }
  }
}

namespace genv {
env_t global;
auto init_global_environment() -> void {
  // an empty frame with nothing
  global.frames.push_front(std::make_shared<frame_t>());
}

auto get_global_environment() -> env_t { return global; }
} // namespace genv
