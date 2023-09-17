#include "internal/Gen.h"

using namespace namec;

void RawExpr::emit_impl(std::ostream &SS) { SS << get_val(); }

void VariableExpr::emit_impl(std::ostream &SS) { SS << get_decl()->get_name(); }

void SubscriptExpr::emit_impl(std::ostream &SS) {
  get_array()->emit(SS);
  SS << "[";
  get_index()->emit(SS);
  SS << "]";
}

void CallExpr::emit_impl(std::ostream &SS) {
  get_callee()->emit(SS);
  SS << "(";
  for (auto I = 0; I < Args.size(); ++I) {
    Args[I]->emit(SS);
    if (I != Args.size() - 1)
      SS << ",";
  }
  SS << ")";
}

void UnaryOp::emit_impl(std::ostream &SS) {
  if (IsPrefix) {
    SS << Op;
    Operand->emit(SS);
  } else {
    Operand->emit(SS);
    SS << Op;
  }
}

void BinaryOp::emit_impl(std::ostream &SS) {
  LHS->emit(SS);
  SS << Op;
  RHS->emit(SS);
}

void TernaryOp::emit_impl(std::ostream &SS) {
  Cond->emit(SS);
  SS << "?";
  Then->emit(SS);
  SS << ":";
  Else->emit(SS);
}

void CastExpr::emit_impl(std::ostream &SS) {
  SS << "(";
  get_type()->emit(SS);
  SS << ")";
  get_operand()->emit(SS);
}

void ParenExpr::emit_impl(std::ostream &SS) {
  SS << "(";
  get_inside()->emit(SS);
  SS << ")";
}
