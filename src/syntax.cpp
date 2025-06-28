#include "syntax.hh"

#include "exp.hh"

namespace syntax {
auto is_self_evaluating(const exp *e) -> bool {
  return dynamic_cast<const exp_number *>(e) != nullptr ||
    dynamic_cast<const exp_string *>(e) != nullptr;
}
} // namespace syntax
