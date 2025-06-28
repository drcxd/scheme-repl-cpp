#include "eval.hh"

#include <cassert>

#include "exp.hh"
#include "syntax.hh"

auto eval(const exp *e) -> std::unique_ptr<exp>
{
  if (syntax::is_self_evaluating(e)) {
    return e->duplicate();
  }
  assert(false); // unknown experssion type
  return nullptr;
}
