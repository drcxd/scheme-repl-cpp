#include "exp.hh"

#include <cassert>
#include <stdexcept>
#include <sstream>

static auto is_digit(const std::string_view& str) -> bool {
  bool ret = true && !str.empty();
  for (int i = 0; i < str.length() && ret; ++i) {
    ret = ret && std::isdigit(str[i]);
  }
  return ret;
}

static auto is_string(const std::string_view &str) -> bool {
  return !str.empty() && str.starts_with('"') && str.ends_with('"');
}

// begin must point to an opening parenthesis, if no matching closing
// parenthesis, then return end. If found matching closing
// parenthesis, return the iterator that points to it.
static auto find_matching_closing_paren(std::string_view::const_iterator begin,
                                        std::string_view::const_iterator end)
    -> std::string_view::const_iterator {
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

// str must begins with ( and ends with )
static auto parse_list(const std::string_view& str) -> std::unique_ptr<exp> {
  auto new_exp = std::make_unique<exp_list>();
  auto start = str.begin() + 1;
  const auto closing_paren_pos = str.end() - 1;
  // scan the elements of the list
  while (start < closing_paren_pos) {
    // skip all spaces
    while (*start == ' ') { ++start; }
    auto end = start + 1;
    // is this a list
    if (*start == '(') {
      // search for its matching closing parenthesis
      end = find_matching_closing_paren(start, str.end());
      if (end == str.end()) {
        throw std::runtime_error("malformed expression");
      }
      ++end;
      auto result = exp::parse({start, end});
      if (result.get() != nullptr) { // handle the case of an empty list
        new_exp->list.push_back(std::move(result));
      }
    } else {
      // must be a number or a string search for a space or the closing parenthesis
      while (*end != ' ' && end < closing_paren_pos) {
        ++end;
      }
      new_exp->list.push_back(exp::parse({start, end}));
    }
    // now ends either points to a closing parenthesis or a space
    start = end + 1;
  }
  return new_exp;
}

auto exp::is_self_evaluating(exp *e) -> bool {
  return dynamic_cast<exp_number *>(e) != nullptr ||
         dynamic_cast<exp_string *>(e) != nullptr;
}

auto exp::parse(const std::string_view& str) -> std::unique_ptr<exp> {
  if (str.empty()) {
    return nullptr;
  }
  if (is_digit(str)) {
    auto new_exp = std::make_unique<exp_number>();
    new_exp->n = std::atoi(str.data());
    return new_exp;
  }
  if (is_string(str)) {
    auto new_exp = std::make_unique<exp_string>();
    new_exp->str = {str.begin() + 1, str.end() - 1};
    return new_exp;
  }
  assert(str.starts_with('(') && str.ends_with(')')); // must be a list
  return parse_list(str);
}

auto exp_number::to_string() const -> std::string { return std::to_string(n); }

auto exp_string::to_string() const -> std::string {
  std::stringstream ss;
  ss << '"' << str << '"';
  return ss.str();
}

auto exp_list::to_string() const -> std::string {
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
