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

class GlobalVariable {
public:
  GlobalVariable(std::string name, std::unique_ptr<Type> type)
      : name(name), type(std::move(type)) {}
  virtual llvm::Value* accept(Visitor& visitor) const;
  const std::string& get_name() const { return name; }
  const Type& get_type() const { return *type; }
private:
  std::string name;
  std::unique_ptr<Type> type;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
