#include "internal/Gen.h"

using namespace namec;

/// @brief
/// @param Name
/// @param RetTy
/// @param Params
/// @param IsVarArg
/// @return
FuncDecl *TopLevel::def_func(std::string Name, Type *RetTy,
                             std::vector<VarDecl *> Params, bool IsVarArg) {
  auto *F = C.decl_func(Name, RetTy, Params, IsVarArg);
  Entries.push_back(F);
  return F;
}

FuncSplitDecl *TopLevel::def_func_declare(std::string Name, Type *RetTy,
                                          std::vector<VarDecl *> Params,
                                          bool IsVarArg) {
  auto *FD = C.decl_func_split(Name, RetTy, Params, IsVarArg);
  auto *Fwd = C.decl_func_split_forward(FD);
  Entries.push_back(Fwd);
  return FD;
}

void TopLevel::def_func_define(FuncSplitDecl *D) { Entries.push_back(D); }

void TopLevel::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    E->emit(SS);
    SS << "\n";
  }
}

void Scope::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    SS << E;
  }
}

void MacroFuncScope::emit_impl(std::ostream &SS) {
  std::stringstream LocalSS;
  for (auto *E : Entries) {
    LocalSS << E;
  }
  std::string LocalStr = LocalSS.str();
  // Replace all newline with newline in macro
  size_t Pos = 0;
  while ((Pos = LocalStr.find("\n", Pos)) != std::string::npos) {
    LocalStr.replace(Pos, 1, "\\\n");
    Pos += 2;
  }
  SS << LocalStr;
}
