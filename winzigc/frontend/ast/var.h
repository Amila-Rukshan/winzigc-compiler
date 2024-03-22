#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/location.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor;

class Variable {
public:
  Variable(SourceLocation location = {0, 0}) : location(location) {}
  virtual ~Variable() = default;
  virtual llvm::Value* accept(Visitor& visitor) const PURE;
  int get_line() const { return location.line; }
  int get_column() const { return location.column; }

private:
  SourceLocation location;
};

class GlobalVariable : public Variable {
public:
  GlobalVariable(SourceLocation location, std::string name, std::unique_ptr<Type> type)
      : Variable(location), name(name), type(std::move(type)) {}
  virtual llvm::Value* accept(Visitor& visitor) const override;
  const std::string& get_name() const { return name; }
  const Type& get_type() const { return *type; }

private:
  std::string name;
  std::unique_ptr<Type> type;
};

class LocalVariable : public Variable {
public:
  LocalVariable(SourceLocation location, std::string name, std::unique_ptr<Type> type)
      : Variable(location), name(name), type(std::move(type)) {}
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
