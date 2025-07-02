#pragma once

#include <list>
#include <memory>

template<typename T>
using uptr = std::unique_ptr<T>;
template <typename T>
using sptr = std::shared_ptr<T>;

struct obj_t;
using record_t = std::pair<std::string, uptr<obj_t>>;
using frame_t = std::list<sptr<record_t>>;
using env_t = std::list<sptr<frame_t>>;
