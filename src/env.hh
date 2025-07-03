#pragma once

#include "typedef.hh"

struct obj_t;

namespace env {
auto init_global_environment() -> void;
auto get_global_environment() -> env_t;
auto get_current_environment() -> env_t;
auto set_current_environment(env_t env) -> void;
auto extend_env(env_t env, const std::list<sptr<obj_t>> &params,
                const std::list<sptr<obj_t>> &values) -> env_t;
auto lookup_variable_value(const std::string_view &var, env_t env)
    -> sptr<obj_t>;
auto define_variable(const std::string_view &var, const sptr<obj_t> &value,
                     env_t env) -> void;
} // namespace env
