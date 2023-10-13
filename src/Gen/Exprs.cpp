#include "internal/Gen.h"

using namespace namec;

void RawExpr::emit_impl(std::ostream &SS) { SS << get_val(); }

void VariableExpr::emit_impl(std::ostream &SS) { SS << get_decl()->get_name(); }

void SubscriptExpr::emit_impl(std::ostream &SS) {
  SS << get_array() << "[" << get_index() << "]";
}

void CallExpr::emit_impl(std::ostream &SS) {
  SS << get_callee() << "(" << join(args()) << ")";
}

void UnaryOp::emit_impl(std::ostream &SS) {
  if (IsPrefix) {
    SS << Op << Operand;
  } else {
    SS << Operand << Op;
  }
}

void BinaryOp::emit_impl(std::ostream &SS) { SS << LHS << Op << RHS; }

void TernaryOp::emit_impl(std::ostream &SS) {
  SS << Cond << "?" << Then << ":" << Else;
}

void CastExpr::emit_impl(std::ostream &SS) {
  SS << "(" << get_type() << ")" << get_operand();
}

void ParenExpr::emit_impl(std::ostream &SS) {
  SS << "(" << get_inside() << ")";
}

void DesignatedInitExpr::emit_impl(std::ostream &SS) {
  SS << "{";
  for (auto I = 0; I < Designators.size(); ++I) {
    auto &[Name, E] = Designators[I];
    SS << "." << Name << "=" << E;
    if (I != Designators.size() - 1) {
      SS << ",";
    }
  }
  SS << "}";
}

void InitListExpr::emit_impl(std::ostream &SS) {
  SS << "{" << join(values()) << "}";
}

void GenericSelection::emit_impl(std::ostream &SS) {
  SS << "_Generic(" << get_control() << ",";
  for (auto I = 0; I < AssocList.size(); ++I) {
    auto &[T, E] = AssocList[I];
    if (T) {
      SS << T << ":";
    } else {
      SS << "default:";
    }
    SS << E;
    if (I != AssocList.size() - 1) {
      SS << ",";
    }
  }
  SS << ")";
}
