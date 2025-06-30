#include "exp.hh"
#include "env.hh"

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <format>

static auto is_number(const std::string_view& str) -> bool {
  bool ret = true && !str.empty();
  int decimal = 0;
  for (int i = 0; i < str.length() && ret && decimal < 2; ++i) {
    if (str[i] == '.') {
      ++decimal;
    } else {
      ret = ret && std::isdigit(str[i]);
    }
  }
  return ret && decimal < 2;
}

static auto is_string(const std::string_view &str) -> bool {
  return !str.empty() && str.starts_with('"') && str.ends_with('"');
}

static auto is_list(const std::string_view &str) -> bool {
  return !str.empty() && str.starts_with('(') && str.ends_with(')');
}

static auto is_quoted(const std::string_view &str) -> bool {
  bool is_empty = str.empty();
  bool begin_quote = str.starts_with('\'');
  bool has_space = false;
  for (auto c : str) {
    if (c == ' ') {
      has_space = true;
      break;
    }
  }
  return !is_empty && begin_quote && !has_space;
}

static auto is_symbol(const std::string_view &str) -> bool {
  return str.find(' ') == std::string_view::npos;
}

static auto parse_quoted(const std::string_view& str) -> uptr<exp_t> {
  assert(str.starts_with('\''));
  auto result = std::make_unique<exp_quoted_t>();
  result->inner_exp = exp_t::parse({str.cbegin() + 1, str.cend()});
  return result;
}

static auto parse_string(const std::string_view& str) -> uptr<exp_t> {
  assert(str.starts_with('"') && str.ends_with('"'));
  auto exp = std::make_unique<exp_string_t>();
  exp-> str = {str.begin() + 1, str.end() - 1};
  return exp;
}

static auto parse_number(const std::string_view& str) -> uptr<exp_t> {
  auto exp = std::make_unique<exp_number_t>();
  exp->n = std::atof(str.data());
  return exp;
}

static auto parse_symbol(const std::string_view& str) -> uptr<exp_t> {
  auto exp = std::make_unique<exp_symbol_t>();
  exp->str = str;
  return exp;
}

// begin must point to an opening parenthesis, if no matching closing
// parenthesis, then return end. If found matching closing
// parenthesis, return the iterator that points to it.
static auto find_matching_closing_paren(const std::string_view &str)
    -> std::string_view::const_iterator {
  assert(str.starts_with('('));
  auto begin = str.cbegin();
  auto end = str.cend();
  auto it = begin + 1;
  int count = 1;
  while (count > 0 && it < end) {
    if (*it == '(') {
      ++count;
    } else if (*it == ')') {
      --count;
    }
    ++it;
  }
  if (count > 0) {
    return end;
  }
  return it - 1;
}

static auto partition_list(const std::string_view &str)
    -> std::vector<std::string_view> {
  assert(str.starts_with('(') && str.ends_with(')'));
  auto it = str.begin() + 1;
  std::vector<std::string_view> elements;
  auto end_paren = str.end() - 1;
  while (it < end_paren) {
    while (*it == ' ') {
      ++it;
    }
    if (*it == '(') {
      // we have a list
      auto end = find_matching_closing_paren({it, end_paren});
      if (end == end_paren) {
        throw std::runtime_error(std::format("malformed expression %s", str));
      }
      elements.push_back({it, end + 1});
      it = end + 1;
    } else {
      // a symbol, number, or a string
      auto end = it + 1;
      while (*end != ' ' && end < end_paren) {
        ++end;
      }
      elements.push_back({it, end});
      it = end + 1;
    }
  }
  return elements;
}

// str must begins with ( and ends with )
static auto parse_list(const std::string_view& str) -> uptr<exp_list_t> {
  assert(str.starts_with('(') && str.ends_with(')'));

  std::vector<std::string_view> elements = partition_list(str);
  // partition elements
  // auto new_list = std::make_unique<exp_list_t>();
  uptr<exp_list_t> exp;
  bool first = true;
  for (auto& element : elements) {
    uptr<exp_t> elem_exp = exp_t::parse(element);
    // checking for keywords, keywords are special symbols now
    if (first) {
      if (elem_exp->to_string() == "define") {
        exp = std::make_unique<exp_define_t>();
      } else {
        exp = std::make_unique<exp_list_t>();
      }
    }
    exp->list.push_back(std::move(elem_exp));
    first = false;
  }
  return exp;
}

auto exp_t::parse(const std::string_view& str) -> uptr<exp_t> {
  if (str.empty()) {
    return nullptr;
  }
  if (is_number(str)) {
    return parse_number(str);
  }
  if (is_string(str)) {
    return parse_string(str);
  }
  if (is_quoted(str)) {
    return parse_quoted(str);
  }
  if (is_list(str)) {
    return parse_list(str);
  }
  if (is_symbol(str)) {
    return parse_symbol(str);
  }
  assert(false); // unknown expression type
  return nullptr;
}

auto exp_number_t::to_string() const -> std::string { return std::to_string(n); }

auto exp_number_t::duplicate() const -> uptr<exp_t> {
  auto copy = std::make_unique<exp_number_t>();
  copy->n = n;
  return copy;
}

auto exp_number_t::eval() -> uptr<exp_t> {
  return duplicate();
}

auto exp_string_t::to_string() const -> std::string {
  std::stringstream ss;
  ss << '"' << str << '"';
  return ss.str();
}

auto exp_string_t::duplicate() const -> uptr<exp_t> {
  auto copy = std::make_unique<exp_string_t>();
  copy->str = str;
  return copy;
}

auto exp_string_t::eval() -> uptr<exp_t> {
  return duplicate();
}

auto exp_list_t::to_string() const -> std::string {
  std::stringstream ss;
  ss << "(";
  for (auto it = list.cbegin(); it != list.cend(); ++it) {
    ss << (*it)->to_string();
    auto test = it;
    if (++test != list.cend()) {
      ss << " ";
    }
  }
  ss << ")";
  return ss.str();
}

auto exp_list_t::duplicate() const -> uptr<exp_t> {
  auto copy = std::make_unique<exp_list_t>();
  copy->list = list;
  return copy;
}

auto exp_quoted_t::to_string() const -> std::string {
  std::stringstream ss;
  ss << '\'' << inner_exp->to_string();
  return ss.str();
}

auto exp_quoted_t::duplicate() const -> uptr<exp_t> {
  auto copy = std::make_unique<exp_quoted_t>();
  copy->inner_exp = inner_exp->duplicate();
  return copy;
}

auto exp_quoted_t::eval() -> uptr<exp_t> {
  return duplicate();
}

auto exp_symbol_t::to_string() const -> std::string {
  return str;
}

auto exp_symbol_t::duplicate() const -> uptr<exp_t> {
  auto copy = std::make_unique<exp_symbol_t>();
  copy->str = str;
  return copy;
}

auto exp_symbol_t::eval() -> uptr<exp_t> {
  auto* env = genv::get_global_environment();
  auto* ret = env->lookup_variable_value(str);
  if (ret != nullptr) {
    return ret->duplicate();
  }
  auto error = std::make_unique<exp_string_t>();
  error->str = std::format("ERROR: undefined symbol {0}", str);
  return error;
}

auto exp_define_t::eval() -> uptr<exp_t> {
  assert(!list.empty());
  auto it = list.begin();
  auto& keyword = *it;
  assert(keyword->to_string() == "define");
  ++it;
  auto* env = genv::get_global_environment();
  const auto& var = (*it)->to_string();
  ++it;
  auto value = (*it)->eval();
  env->define_variable(var, std::move(value));
  auto ret = std::make_unique<exp_string_t>();
  ret->str = "ok";
  return ret;
}
