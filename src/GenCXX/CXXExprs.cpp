#include "internal/GenCXX.h"

using namespace namecxx;

void RawExpr::emit_impl(std::ostream &SS) { SS << get_val(); }

void VariableExpr::emit_impl(std::ostream &SS) {
  SS << get_decl()->get_name_str();
}

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

void InitListExpr::emit_impl(std::ostream &SS) {
  SS << "{" << join(values()) << "}";
}

void NewExpr::emit_impl(std::ostream &SS) {
  SS << "new ";
  if (get_placement()) {
    SS << "(" << get_placement() << ")";
  }
  SS << get_type();
  if (get_array_size()) {
    SS << "[" << get_array_size() << "]";
  }
  if (IsListInit) {
    SS << "{" << join(args()) << "}";
  } else {
    SS << "(" << join(args()) << ")";
  }
}

void DeleteExpr::emit_impl(std::ostream &SS) {
  SS << "delete ";
  if (is_array()) {
    SS << "[] ";
  }
  SS << get_expr();
}

void UserDefinedLiteral::emit_impl(std::ostream &SS) {
  SS << get_expr() << "_" << get_postfix();
}

void QualNameExpr::emit_impl(std::ostream &SS) { SS << get_name().to_string(); }

LambdaExpr::LambdaExpr(Context &C, std::vector<Expr *> Captures,
                       std::vector<VarDecl *> Params, bool IsVarArgs,
                       Type *RetTy)
    : Captures(Captures), Params(Params), IsVarArgs(IsVarArgs),
      Body(C.add_func_scope()), RetTy(RetTy) {}

void LambdaExpr::emit_impl(std::ostream &SS) {
  SS << "[" << join(captures()) << "]";
  SS << "(" << join(params());
  if (IsVarArgs) {
    SS << ",...";
  }
  SS << ")";
  if (RetTy) {
    SS << "->" << RetTy;
  }
  SS << "{" << Body << "}";
}

void InstantiateExpr::emit_impl(std::ostream &SS) {
  SS << get_template_decl()->get_name_str() << "<" << join(args()) << ">";
}

void PackExpansionExpr::emit_impl(std::ostream &SS) {
  SS << get_expr() << "...";
}

void FoldExpr::emit_impl(std::ostream &SS) {
  if (get_init()) {
    if (IsLeftFold) {
      SS << "(" << get_init() << Op << " ... " << Op << get_pack() << ")";
    } else {
      SS << "(" << get_pack() << Op << " ... " << Op << get_init() << ")";
    }
  } else {
    if (IsLeftFold) {
      SS << "( ... " << Op << get_pack() << ")";
    } else {
      SS << "(" << get_pack() << Op << " ... )";
    }
  }
}
