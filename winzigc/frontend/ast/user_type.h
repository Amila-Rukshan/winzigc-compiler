#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/common/pure.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor;

class UserTypeDef {
public:
  virtual ~UserTypeDef() = default;
  virtual void accept(Visitor& visitor) const PURE;
};

class GlobalUserTypeDef : public UserTypeDef {
public:
  GlobalUserTypeDef(std::string name, std::vector<std::string> value_names)
      : type_name(std::move(name)), value_names(std::move(value_names)) {}
  virtual void accept(Visitor& visitor) const override;
  const std::string& get_type_name() const { return type_name; }
  const std::vector<std::string>& get_value_names() const { return value_names; }

private:
  std::string type_name;
  std::vector<std::string> value_names;
};

class LocalUserTypeDef : public UserTypeDef {
public:
  LocalUserTypeDef(std::string name, std::vector<std::string> value_names)
      : type_name(std::move(name)), value_names(std::move(value_names)) {}
  virtual void accept(Visitor& visitor) const override;
  const std::string& get_type_name() const { return type_name; }
  const std::vector<std::string>& get_value_names() const { return value_names; }

private:
  std::string type_name;
  std::vector<std::string> value_names;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
