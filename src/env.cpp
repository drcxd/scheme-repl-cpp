#include "env.hh"

#include <cassert>

#include "exp.hh"
#include "procedure.hh"

namespace env {
std::list<sptr<frame_t>> global;
env_t curenv;

static auto add_to_frame(sptr<frame_t> &frame, const std::string_view &name,
                         const sptr<obj_t> &value) {
  frame->push_back(std::make_shared<record_t>(std::make_pair(name, value)));
}

auto init_global_environment() -> void {
  global.clear();
  auto base_frame = std::make_shared<frame_t>();
  add_to_frame(base_frame, "+", std::make_shared<proc_add_t>());
  add_to_frame(base_frame, "-", std::make_shared<proc_min_t>());
  add_to_frame(base_frame, "*", std::make_shared<proc_mul_t>());
  add_to_frame(base_frame, "/", std::make_shared<proc_div_t>());
  add_to_frame(base_frame, "<", std::make_shared<proc_lt_t>());
  add_to_frame(base_frame, ">", std::make_shared<proc_gt_t>());
  add_to_frame(base_frame, "cons", std::make_shared<proc_cons_t>());
  global.push_front(base_frame);
  curenv = global;
}

auto get_global_environment() -> env_t { return global; }

auto get_current_environment() -> env_t { return curenv; }

auto lookup_variable_value(const std::string_view &var, env_t env)
    -> sptr<obj_t> {
  for (auto &frame : env) {
    for (auto &record : *frame) {
      if (record->first == var) {
        return record->second;
      }
    }
  }
  return nullptr;
}

auto define_variable(const std::string_view &var, const sptr<obj_t> &value,
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
    add_to_frame(first_frame, var, value);
  }
}

auto set_current_environment(env_t env) -> void { curenv = env; }

auto extend_env(env_t env, const std::list<sptr<obj_t>> &params,
                const std::list<sptr<obj_t>> &values) -> env_t {
  auto new_frame = std::make_shared<frame_t>();
  auto itp = params.begin();
  auto itv = values.begin();
  while (itp != params.end() && itv != values.end()) {
    add_to_frame(new_frame, (*itp)->to_string(), (*itv));
    ++itp;
    ++itv;
  }
  assert(itp == params.end() && itv == values.end());
  env.push_front(new_frame);
  return env;
}

} // namespace env
