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

void DesignatedInitExpr::emit_impl(std::ostream &SS) {
  SS << "{";
  for (auto I = 0; I < Designators.size(); ++I) {
    auto &[Name, E] = Designators[I];
    SS << "." << Name << "=";
    E->emit(SS);
    if (I != Designators.size() - 1) {
      SS << ",";
    }
  }
  SS << "}";
}

void InitListExpr::emit_impl(std::ostream &SS) {
  SS << "{";
  for (auto I = 0; I < Values.size(); ++I) {
    Values[I]->emit(SS);
    if (I != Values.size() - 1) {
      SS << ",";
    }
  }
  SS << "}";
}

void GenericSelection::emit_impl(std::ostream &SS) {
  SS << "_Generic(";
  get_control()->emit(SS);
  SS << ",";
  for (auto I = 0; I < AssocList.size(); ++I) {
    auto &[T, E] = AssocList[I];
    if (T) {
      T->emit(SS);
      SS << ":";
    } else {
      SS << "default:";
    }
    E->emit(SS);
    if (I != AssocList.size() - 1) {
      SS << ",";
    }
  }
  SS << ")";
}
