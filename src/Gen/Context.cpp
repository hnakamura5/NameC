#include "internal/Gen.h"

using namespace namec;

Scope *Context::add_scope() {
  Scope *S = new Scope(*this);
  Scopes.push_back(std::unique_ptr<Scope>(S));
  return S;
}
