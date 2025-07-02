#include "procedure.hh"

#include <format>

#include "exp.hh"

template <typename T>
static auto for_each_numerical_arg(const std::list<sptr<obj_t>> &args, T func)
    -> void {
  for (auto& arg : args) {
    auto ret = arg->eval();
    auto* operand = dynamic_cast<obj_number_t *>(ret.get());
    assert(operand != nullptr);
    func(operand->n);
  }
}

auto proc_t::to_string() const -> std::string {
  return std::format("procedure {}", (void*)this);
}

auto proc_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_t>();
  *copy = *this;
  return copy;
}

auto proc_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  // TODO:
  // evaluate args
  // extend environments
  // evalute body
  // retrun result
  return std::make_unique<obj_number_t>();
}

auto proc_add_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  auto accu = [&n](double num){ n += num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_add_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_add_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_min_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  auto accu = [&n](double num){ n -= num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_min_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_min_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_mul_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  auto accu = [&n](double num){ n *= num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_mul_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_mul_t>();
  copy->proc_t::operator=(*this);
  return copy;
}

auto proc_div_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  auto accu = [&n](double num){ n /= num; };
  for_each_numerical_arg(args, accu);
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_div_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_div_t>();
  copy->proc_t::operator=(*this);
  return copy;
}
