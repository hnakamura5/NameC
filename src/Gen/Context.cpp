#include "internal/Gen.h"

using namespace namec;

FuncScope *Context::add_scope() {
  FuncScope *S = new FuncScope(*this);
  Scopes.push_back(std::unique_ptr<FuncScope>(S));
  return S;
}
