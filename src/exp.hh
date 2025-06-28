#include <list>
#include <memory>
#include <string>

struct exp {
  virtual ~exp() {}
  virtual auto to_string() const -> std::string = 0;

  static auto parse(const std::string_view& str) -> std::unique_ptr<exp>;
  static auto is_self_evaluating(exp* e) -> bool;
};

// we only support integer operation for now
struct exp_number : public exp {
  auto to_string() const -> std::string override;
  double n = 0;
};

struct exp_string : public exp {
  auto to_string() const -> std::string override;
  std::string str;
};

struct exp_symbol : public exp {
  auto to_string() const -> std::string override;
  std::string str;
};

// quoted values are supported as lists: (quote ...)
struct exp_list : public exp {
  auto to_string() const -> std::string override;
  std::list<std::unique_ptr<exp>> list;
};

struct exp_quoted : public exp {
  auto to_string() const -> std::string override;
  std::unique_ptr<exp> inner_exp;
};
