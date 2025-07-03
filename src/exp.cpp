#include "exp.hh"
#include "env.hh"
#include "procedure.hh"

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
  return str.starts_with('\'');
}

static auto is_symbol(const std::string_view &str) -> bool {
  return str.find(' ') == std::string_view::npos;
}

static auto parse_quoted(const std::string_view& str) -> sptr<obj_t> {
  assert(str.starts_with('\''));
  auto result = std::make_shared<obj_quoted_t>();
  result->inner_obj = parse_exp({str.cbegin() + 1, str.cend()});
  return result;
}

static auto parse_string(const std::string_view& str) -> sptr<obj_t> {
  assert(str.starts_with('"') && str.ends_with('"'));
  auto obj = std::make_shared<obj_string_t>();
  obj-> str = {str.begin() + 1, str.end() - 1};
  return obj;
}

static auto parse_number(const std::string_view& str) -> sptr<obj_t> {
  auto obj = std::make_shared<obj_number_t>();
  obj->n = std::atof(str.data());
  return obj;
}

static auto parse_symbol(const std::string_view& str) -> sptr<obj_t> {
  auto obj = std::make_shared<obj_symbol_t>();
  obj->str = str;
  return obj;
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
static auto parse_list(const std::string_view& str) -> sptr<obj_list_t> {
  assert(str.starts_with('(') && str.ends_with(')'));

  std::vector<std::string_view> elements = partition_list(str);
  // partition elements
  sptr<obj_list_t> obj;
  bool first = true;
  for (auto& element : elements) {
    sptr<obj_t> elem_obj = parse_exp(element);
    // checking for keywords, keywords are special symbols now
    if (first) {
      if (elem_obj->to_string() == "define") {
        obj = std::make_shared<obj_define_t>();
      } else if (elem_obj->to_string() == "if") {
        obj = std::make_shared<obj_branch_t>();
      } else if (elem_obj->to_string() == "lambda") {
        obj = std::make_shared<obj_lambda_t>();
      } else {
        obj = std::make_shared<obj_list_t>();
      }
    }
    obj->list.push_back(std::move(elem_obj));
    first = false;
  }
  return obj;
}

auto parse_exp(const std::string_view& str) -> sptr<obj_t> {
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

auto obj_number_t::to_string() const -> std::string { return std::to_string(n); }

auto obj_number_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<obj_number_t>();
  copy->n = n;
  return copy;
}

auto obj_number_t::eval() -> sptr<obj_t> {
  return shared_from_this();
}

auto obj_string_t::to_string() const -> std::string {
  std::stringstream ss;
  ss << '"' << str << '"';
  return ss.str();
}

auto obj_string_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<obj_string_t>();
  copy->str = str;
  return copy;
}

auto obj_string_t::eval() -> sptr<obj_t> {
  return shared_from_this();
}

auto obj_list_t::to_string() const -> std::string {
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

auto obj_list_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<obj_list_t>();
  copy->list = list;
  return copy;
}

auto obj_list_t::eval() -> sptr<obj_t> {
  assert(!list.empty());
  auto it = list.begin();
  auto* op = dynamic_cast<proc_t *>((*it)->eval().get());
  assert(op != nullptr);
  ++it;
  return op->apply({it, list.end()});
}

auto obj_quoted_t::to_string() const -> std::string {
  std::stringstream ss;
  ss << '\'' << inner_obj->to_string();
  return ss.str();
}

auto obj_quoted_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<obj_quoted_t>();
  copy->inner_obj = inner_obj->duplicate();
  return copy;
}

auto obj_quoted_t::eval() -> sptr<obj_t> {
  return shared_from_this();
}

auto obj_symbol_t::to_string() const -> std::string {
  return str;
}

auto obj_symbol_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<obj_symbol_t>();
  copy->str = str;
  return copy;
}

auto obj_symbol_t::eval() -> sptr<obj_t> {
  if (str == "true" or str == "false") {
    return shared_from_this();
  };
  auto env = env::get_current_environment();
  auto ret = env::lookup_variable_value(str, env);
  if (ret != nullptr) {
    return ret;
  }
  auto error = std::make_shared<obj_string_t>();
  error->str = std::format("ERROR: undefined symbol {0}", str);
  return error;
}

auto obj_define_t::eval() -> sptr<obj_t> {
  assert(!list.empty());
  auto it = list.begin();
  auto& keyword = *it;
  assert(keyword->to_string() == "define");
  ++it;
  auto env = env::get_current_environment();
  const auto& var = (*it)->to_string();
  ++it;
  auto value = (*it)->eval();
  env::define_variable(var, std::move(value), env);
  auto ret = std::make_shared<obj_string_t>();
  ret->str = "ok";
  return ret;
}

auto obj_branch_t::eval() -> sptr<obj_t> {
  assert(!list.empty());
  auto it = list.begin();
  auto& keyword = *it;
  assert(keyword->to_string() == "if");
  ++it;
  auto condition = (*it)->eval();
  ++it;
  if (dynamic_cast<obj_symbol_t *>(condition.get()) == nullptr ||
      condition->to_string() != "false") {
    return (*it)->eval();
  }
  ++it;
  return (*it)->eval();
}

auto obj_lambda_t::duplicate() const -> sptr<obj_t> {
  auto copy = std::make_shared<obj_lambda_t>();
  copy->list = list;
  return copy;
}

auto obj_lambda_t::eval() -> sptr<obj_t> {
  assert(!list.empty());
  auto proc = std::make_shared<proc_t>();

  auto it = list.begin();
  ++it;
  sptr<obj_list_t> params = std::dynamic_pointer_cast<obj_list_t>(*it);
  assert(params.get() != nullptr);
  proc->params = params->list;

  ++it;
  while (it != list.end()) {
    proc->body.push_back(*it);
    ++it;
  }
  return proc;
}
