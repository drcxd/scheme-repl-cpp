#pragma once

struct exp;

namespace syntax {
auto is_self_evaluating(const exp *e) -> bool;
}
