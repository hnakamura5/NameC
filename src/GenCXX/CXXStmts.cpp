#include "internal/GenCXX.h"

using namespace namecxx;

void RawStmt::emit_impl(std::ostream &SS) { SS << get_val(); }

void DeclStmt::emit_impl(std::ostream &SS) { SS << get_decl() << ";"; }

void IfStmt::emit_impl(std::ostream &SS) {
  SS << "if";
  if (is_constexpr()) {
    SS << " constexpr";
  }
  SS << "(";
  if (get_init()) {
    SS << get_init() << ";";
  }
  SS << get_cond() << "){" << get_then() << "}";
  for (auto &[ECond, EBody, EInit] : Elseifs) {
    SS << "else if(";
    if (EInit) {
      SS << EInit << ";";
    }
    SS << ECond << "){" << EBody << "}";
  }
  if (has_else()) {
    SS << "else{" << Else << "}";
  }
}

void WhileStmt::emit_impl(std::ostream &SS) {
  SS << "while(" << get_cond() << "){";
  SS << get_body();
  SS << "}";
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
    SS << get_step();
  }
  SS << "){" << get_body() << "}";
}

void ForRangeStmt::emit_impl(std::ostream &SS) {
  SS << "for(" << get_decl() << ":" << get_range() << "){";
  SS << get_body();
  SS << "}";
}

void DoStmt::emit_impl(std::ostream &SS) {
  SS << "do{";
  SS << get_body();
  SS << "}while(" << get_cond() << ")";
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
  SS << get_label_name() << ":";
  if (get_stmt()) {
    SS << get_stmt();
  } else {
    SS << ";";
  }
}

void GotoStmt::emit_impl(std::ostream &SS) {
  SS << "goto " << get_label_name() << ";";
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
    SS << C;
  }
  SS << "}";
}

void ThrowStmt::emit_impl(std::ostream &SS) {
  SS << "throw";
  if (get_expr()) {
    SS << " " << get_expr();
  }
  SS << ";";
}

void TryStmt::emit_impl(std::ostream &SS) {
  SS << "try{" << get_body() << "}";
  for (auto &[CatchVar, Handler] : Handlers) {
    SS << "catch(";
    if (CatchVar) {
      SS << CatchVar;
    } else {
      SS << "...";
    }
    SS << "){" << Handler << "}";
  }
}

void UsingNamespaceStmt::emit_impl(std::ostream &SS) {
  SS << "using namespace " << get_name_str() << ";";
}
