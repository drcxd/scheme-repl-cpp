#pragma once

#include <memory>

struct exp;

auto eval(const exp *e) -> std::unique_ptr<exp>;
