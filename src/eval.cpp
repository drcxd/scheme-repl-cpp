#include "eval.hh"

#include "exp.hh"
#include "syntax.hh"
#include <cassert>

auto eval(const exp *e) -> std::unique_ptr<exp>
{
  if (syntax::is_self_evaluating(e)) {
    if (auto* expn = dynamic_cast<const exp_number *>(e)) {
      auto ret = std::make_unique<exp_number>();
      ret->n = expn->n;
      return ret;
    }
    if (auto* exps = dynamic_cast<const exp_string *>(e)) {
      auto ret = std::make_unique<exp_string>();
      ret->str = exps->str;
      return ret;
    }
  }
  assert(false); // unknown experssion type
  return nullptr;
}
