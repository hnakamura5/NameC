#include "internal/Gen.h"

using namespace namec;

ForStmt *Scope::stmt_for(VarDecl *Init, Expr *Cond, Expr *Step) {
  auto InitStmt = std::make_unique<DeclStmt>(C, Init);
  ForStmt *S = new ForStmt(C, std::move(InitStmt), Cond, Step);
  return add(S);
}

ForStmt *Scope::stmt_for(Expr *Init, Expr *Cond, Expr *Step) {
  auto InitStmt = std::make_unique<ExprStmt>(C, Init);
  ForStmt *S = new ForStmt(C, std::move(InitStmt), Cond, Step);
  return add(S);
}

void Scope::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    E->emit(SS);
  }
}
