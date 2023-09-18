#include "internal/Gen.h"

#include <fstream>

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

void CFile::emit_to_file(std::filesystem::path Path) {
  std::ofstream OS(Path);
  emit(OS);
}

void CFile::emit_impl(std::ostream &SS) {
  for (auto &T : TopLevels) {
    T->emit(SS);
    SS << "\n";
  }
}
