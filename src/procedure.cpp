#include "procedure.hh"

#include <format>

#include "exp.hh"
#include "env.hh"

static auto get_number_checked(const sptr<obj_t>& v) -> double {
  auto* num = dynamic_cast<obj_number_t *>(v.get());
  assert(num != nullptr);
  return num->n;
}

template <typename T>
static auto for_each_numerical_arg(const std::list<sptr<obj_t>> &args, T func)
    -> void {
  for (auto& arg : args) {
    auto ret = arg->eval();
    func(get_number_checked(ret));
  }
}

template <typename T>
static auto list_len(const std::list<T>& l) -> int {
  int n = 0;
  for (auto it = l.begin(); it != l.end(); ++it) {
    ++n;
  }
  return n;
}

auto proc_t::to_string() const -> std::string {
  return std::format("procedure {}", (void*)this);
}

auto proc_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<proc_t>();
  *copy = *this;
  return copy;
}

auto proc_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  std::list<sptr<obj_t>> arg_values;
  for (auto& arg : args) {
    arg_values.push_back(arg->eval());
  }
  env_t cached_env = env::get_current_environment();
  env_t extended_env = env::extend_env(cached_env, params, arg_values);
  env::set_current_environment(extended_env);
  sptr<obj_t> result;
  auto it = body.begin();
  while (it != body.end()) {
    auto v = (*it)->eval();
    ++it;
    if (it == body.end()) {
      result = v;
    }
  }
  env::set_current_environment(cached_env);
  return result;
}

auto proc_add_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  double n = 0;
  auto accu = [&n](double num){ n += num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_shared<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_add_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<proc_add_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_min_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  int len = list_len(args);
  double n = 0;
  auto accu = [&n](double num) { n -= num; };
  if (len <= 1) {
    for_each_numerical_arg(args, accu);
  } else {
    n = get_number_checked(*args.begin());
    for_each_numerical_arg({++args.begin(), args.end()}, accu);
  }
  auto ret = std::make_shared<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_min_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<proc_min_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_mul_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  double n = 1;
  auto accu = [&n](double num){ n *= num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_shared<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_mul_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<proc_mul_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_div_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  double n = 1;
  auto accu = [&n](double num){ n /= num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_shared<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_div_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<proc_div_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_lt_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  assert(!args.empty());
  auto it = args.begin();
  double op1 = get_number_checked(*it);
  ++it;
  auto op2 = get_number_checked(*it);
  auto ret = std::make_shared<obj_symbol_t>();
  ret->str = op1 < op2 ? "true" : "false";
  return ret;
}

auto proc_gt_t::apply(const std::list<sptr<obj_t>> &args) -> sptr<obj_t> {
  assert(!args.empty());
  auto it = args.begin();
  double op1 = get_number_checked(*it);
  ++it;
  auto op2 = get_number_checked(*it);
  auto ret = std::make_shared<obj_symbol_t>();
  ret->str = op1 > op2 ? "true" : "false";
  return ret;
}
