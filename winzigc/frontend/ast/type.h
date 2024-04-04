#pragma once

#include <memory>
#include <string>

#include "winzigc/frontend/ast/type.h"
#include "winzigc/common/pure.h"

#include "llvm/IR/Type.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor;

class Type {
public:
  virtual ~Type() = default;
  virtual void accept(Visitor& visitor) const PURE;
};

class IntegerType : public Type {
public:
  IntegerType() = default;
  virtual void accept(Visitor& visitor) const override;
};

class BooleanType : public Type {
public:
  BooleanType() = default;
  virtual void accept(Visitor& visitor) const override;
};

class CharacterType : public Type {
public:
  CharacterType() = default;
  virtual void accept(Visitor& visitor) const override;
};

class UserType : public Type {
public:
  UserType() = default;
  virtual void accept(Visitor& visitor) const override;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
