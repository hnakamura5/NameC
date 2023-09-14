#include "internal/Gen.h"

using namespace namec;

void RawExpr::emit_impl(std::stringstream &SS) { SS << get_val(); }

void Variable::emit_impl(std::stringstream &SS) {
  SS << get_decl()->get_name();
}

void Subscript::emit_impl(std::stringstream &SS) {
  get_array()->emit(SS);
  SS << "[";
  get_index()->emit(SS);
  SS << "]";
}

void Call::emit_impl(std::stringstream &SS) {
  get_callee()->emit(SS);
  SS << "(";
  for (auto Arg : get_args()) {
    Arg->emit(SS);
    SS << ", ";
  }
  SS << ")";
}

void UnaryOp::emit_impl(std::stringstream &SS) {
  if (IsPrefix) {
    SS << Op;
    Operand->emit(SS);
  } else {
    Operand->emit(SS);
    SS << Op;
  }
}

void BinaryOp::emit_impl(std::stringstream &SS) {
  LHS->emit(SS);
  SS << Op;
  RHS->emit(SS);
}

void TernaryOp::emit_impl(std::stringstream &SS) {
  Cond->emit(SS);
  SS << " ? ";
  Then->emit(SS);
  SS << " : ";
  Else->emit(SS);
}

void Cast::emit_impl(std::stringstream &SS) {
  SS << "(";
  get_type()->emit(SS);
  SS << ")";
  get_operand()->emit(SS);
}

void Paren::emit_impl(std::stringstream &SS) {
  SS << "(";
  get_inside()->emit(SS);
  SS << ")";
}
