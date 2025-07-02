#include "procedure.hh"

#include <format>

#include "exp.hh"

auto proc_t::to_string() const -> std::string {
  return std::format("procedure {}", (void*)this);
}

auto proc_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_t>();
  copy->params = params;
  copy->body = body;
  copy->env = env;
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
  for (auto& exp : args) {
    auto ret = exp->eval();
    auto* operand = dynamic_cast<obj_number_t *>(ret.get());
    assert(operand != nullptr); // TODO: throw exception and catch
    n += operand->n;
  }
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_add_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_add_t>();
  copy->params = params;
  copy->body = body;
  copy->env = env;
  return copy;
}

auto proc_min_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  for (auto& exp : args) {
    auto ret = exp->eval();
    auto* operand = dynamic_cast<obj_number_t *>(ret.get());
    assert(operand != nullptr); // TODO: throw exception and catch
    n -= operand->n;
  }
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_min_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_min_t>();
  copy->params = params;
  copy->body = body;
  copy->env = env;
  return copy;
}

auto proc_mul_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  for (auto& exp : args) {
    auto ret = exp->eval();
    auto* operand = dynamic_cast<obj_number_t *>(ret.get());
    assert(operand != nullptr); // TODO: throw exception and catch
    n *= operand->n;
  }
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_mul_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_mul_t>();
  copy->params = params;
  copy->body = body;
  copy->env = env;
  return copy;
}

auto proc_div_t::apply(const std::list<sptr<obj_t>> &args) -> uptr<obj_t> {
  double n = 0;
  for (auto& exp : args) {
    auto ret = exp->eval();
    auto* operand = dynamic_cast<obj_number_t *>(ret.get());
    assert(operand != nullptr); // TODO: throw exception and catch
    n /= operand->n;
  }
  auto ret = std::make_unique<obj_number_t>();
  ret->n = n;
  return ret;
}

auto proc_div_t::duplicate() const -> uptr<obj_t> {
  auto copy = std::make_unique<proc_div_t>();
  copy->params = params;
  copy->body = body;
  copy->env = env;
  return copy;
}
