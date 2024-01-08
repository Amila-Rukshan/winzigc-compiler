#include <memory>
#include <string>

#include "winzigc/frontend/ast/expr.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Type {
public:
  virtual ~Type() = default;
};

class IntegerType : public Type {
public:
  IntegerType() = default;
};

class BooleanType : public Type {
public:
  BooleanType() = default;
};

class VoidType : public Type {
public:
  VoidType() = default;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
