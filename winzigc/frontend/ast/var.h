#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/var.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor;

class Variable {
public:
  virtual ~Variable() = default;
  virtual llvm::Value* accept(Visitor& visitor) const PURE;
};

class GlobalVariable : public Variable {
public:
  GlobalVariable(std::string name, std::unique_ptr<Type> type)
      : name(name), type(std::move(type)) {}
  virtual llvm::Value* accept(Visitor& visitor) const override;
  const std::string& get_name() const { return name; }
  const Type& get_type() const { return *type; }

private:
  std::string name;
  std::unique_ptr<Type> type;
};

class LocalVariable : public Variable {
public:
  LocalVariable(std::string name, std::unique_ptr<Type> type) : name(name), type(std::move(type)) {}
  virtual llvm::Value* accept(Visitor& visitor) const override;
  const std::string& get_name() const { return name; }
  const Type& get_type() const { return *type; }

private:
  std::string name;
  std::unique_ptr<Type> type;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
