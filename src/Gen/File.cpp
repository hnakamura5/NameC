#include "internal/Gen.h"

using namespace namec;

FuncDecl *TopLevel::def_func(std::string Name, Type *RetTy,
                             std::vector<VarDecl *> Params) {
  auto *F = C.decl_func(Name, RetTy, Params);
  Entries.push_back(F);
  return F;
}

void TopLevel::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    E->emit(SS);
    SS << "\n";
  }
}

void File::emit_impl(std::ostream &SS) { TheTopLevel->emit(SS); }
