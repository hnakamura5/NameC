#include "internal/GenCXX.h"

using namespace namecxx;

FuncDecl *TopLevel::def_func(QualName Name, Type *RetTy,
                             std::vector<VarDecl *> Params, bool IsVarArg) {
  auto *F = C.decl_func(Name, RetTy, Params, IsVarArg);
  Entries.push_back(F);
  return F;
}

TopLevel *TopLevel::def_namespace(QualName Name) {
  auto *NS = new Namespace(C, Name);
  Namespaces.push_back(std::unique_ptr<Namespace>(NS));
  Entries.push_back(NS);
  return NS->get_body();
}

void TopLevel::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    SS << E << "\n";
  }
}

void FuncScope::emit_impl(std::ostream &SS) {
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

TopLevel *ClassTopLevel::def_namespace(QualName Name) {
  auto *NS = new Namespace(C, Name);
  Namespaces.push_back(std::unique_ptr<Namespace>(NS));
  Entries.push_back(NS);
  return NS->get_body();
}
