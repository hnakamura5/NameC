#include "internal/Gen.h"

using namespace namec;

void RawStmt::emit_impl(std::ostream &SS) { SS << get_val(); }

void DeclStmt::emit_impl(std::ostream &SS) { SS << get_decl() << ";"; }

void IfStmt::emit_impl(std::ostream &SS) {
  SS << "if(" << get_cond() << "){" << get_then() << "}";
  for (auto &[ECond, EBody] : Elseifs) {
    SS << "else if(" << ECond << "){" << EBody << "}";
  }
  if (has_else()) {
    SS << "else{" << Else << "}";
  }
}

void WhileStmt::emit_impl(std::ostream &SS) {
  SS << "while(" << get_cond() << "){" << get_body() << "}";
}

void ForStmt::emit_impl(std::ostream &SS) {
  SS << "for(";
  if (get_init()) {
    SS << get_init();
    // Here Init Stmt is VarStmt or ExprStmt. Semicolon is emitted by it.
  } else {
    SS << ";";
  }
  if (get_cond()) {
    SS << get_cond() << ";";
  } else {
    SS << ";";
  }
  if (get_step()) {
    get_step()->emit(SS);
  }
  SS << "){" << get_body() << "}";
}

void DoStmt::emit_impl(std::ostream &SS) {
  SS << "do{" << get_body() << "}while(" << get_cond() << ")";
}

void BlockStmt::emit_impl(std::ostream &SS) { SS << "{" << get_scope() << "}"; }

void ExprStmt::emit_impl(std::ostream &SS) { SS << get_expr() << ";"; }

void ReturnStmt::emit_impl(std::ostream &SS) {
  SS << "return";
  if (get_expr()) {
    SS << " " << get_expr();
  }
  SS << ";";
}

void BreakStmt::emit_impl(std::ostream &SS) { SS << "break;"; }

void ContinueStmt::emit_impl(std::ostream &SS) { SS << "continue;"; }

void LabelStmt::emit_impl(std::ostream &SS) {
  SS << get_name() << ":";
  if (get_stmt()) {
    SS << get_stmt();
  } else {
    SS << ";";
  }
}

void GotoStmt::emit_impl(std::ostream &SS) {
  SS << "goto " << get_name() << ";";
}

void CaseStmt::emit_impl(std::ostream &SS) {
  if (get_val()) {
    SS << "case " << get_val() << ":";
  } else {
    SS << "default:";
  }
  SS << get_body();
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
  SS << "switch(" << get_cond() << "){";
  for (auto &C : Cases) {
    C->emit(SS);
  }
  SS << "}";
}
