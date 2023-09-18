#include "internal/Gen.h"

using namespace namec;

void RawStmt::emit_impl(std::ostream &SS) { SS << get_val(); }

void DeclStmt::emit_impl(std::ostream &SS) {
  get_decl()->emit(SS);
  SS << ";";
}

void IfStmt::emit_impl(std::ostream &SS) {
  SS << "if(";
  get_cond()->emit(SS);
  SS << "){";
  get_then()->emit(SS);
  SS << "}";
  for (auto &[ECond, EBody] : Elseifs) {
    SS << "else if(";
    ECond->emit(SS);
    SS << "){";
    EBody->emit(SS);
    SS << "}";
  }
  if (has_else()) {
    SS << "else{";
    Else->emit(SS);
    SS << "}";
  }
}

void WhileStmt::emit_impl(std::ostream &SS) {
  SS << "while(";
  get_cond()->emit(SS);
  SS << "){";
  get_body()->emit(SS);
  SS << "}";
}

void ForStmt::emit_impl(std::ostream &SS) {
  SS << "for(";
  if (get_init()) {
    get_init()->emit(SS);
    // Here Init Stmt is VarStmt or ExprStmt. Semicolon is emitted by it.
  } else {
    SS << ";";
  }
  if (get_cond()) {
    get_cond()->emit(SS);
    SS << ";";
  } else {
    SS << ";";
  }
  if (get_step()) {
    get_step()->emit(SS);
  }
  SS << "){";
  get_body()->emit(SS);
  SS << "}";
}

void DoStmt::emit_impl(std::ostream &SS) {
  SS << "do{";
  get_body()->emit(SS);
  SS << "}while(";
  get_cond()->emit(SS);
  SS << ")";
}

void BlockStmt::emit_impl(std::ostream &SS) {
  SS << "{";
  get_scope()->emit(SS);
  SS << "}";
}

void ExprStmt::emit_impl(std::ostream &SS) {
  get_expr()->emit(SS);
  SS << ";";
}

void ReturnStmt::emit_impl(std::ostream &SS) {
  SS << "return";
  if (get_expr()) {
    SS << " ";
    get_expr()->emit(SS);
  }
  SS << ";";
}

void BreakStmt::emit_impl(std::ostream &SS) { SS << "break;"; }

void ContinueStmt::emit_impl(std::ostream &SS) { SS << "continue;"; }

void LabelStmt::emit_impl(std::ostream &SS) {
  SS << get_name() << ":";
  if (get_stmt()) {
    get_stmt()->emit(SS);
  } else {
    SS << ";";
  }
}

void GotoStmt::emit_impl(std::ostream &SS) {
  SS << "goto " << get_name() << ";";
}

void CaseStmt::emit_impl(std::ostream &SS) {
  if (get_val()) {
    SS << "\ncase ";
    get_val()->emit(SS);
    SS << ":";
  } else {
    SS << "\ndefault:";
  }
  get_body()->emit(SS);
  if (!IsFallThrough) {
    SS << "break;";
  }
}

CaseStmt *SwitchStmt::add_case(Expr *Val, bool IsFallThrough) {
  auto Case = std::make_unique<CaseStmt>(C, Val, IsFallThrough);
  auto *Ret = Case.get();
  Cases.push_back(std::move(Case));
  return Ret;
}

CaseStmt *SwitchStmt::add_default(bool IsFallThrough) {
  auto Case = std::make_unique<CaseStmt>(C, nullptr, IsFallThrough);
  auto *Ret = Case.get();
  Cases.push_back(std::move(Case));
  return Ret;
}

void SwitchStmt::emit_impl(std::ostream &SS) {
  SS << "switch(";
  get_cond()->emit(SS);
  SS << "){";
  for (auto &C : Cases) {
    C->emit(SS);
  }
  SS << "}";
}
