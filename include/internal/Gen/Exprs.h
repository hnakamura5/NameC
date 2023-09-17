#ifndef NAMEC_GEN_EXPRS_H
#define NAMEC_GEN_EXPRS_H

#include "internal/Gen/Decl.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/Forwards.h"

namespace namec {

class Expr : public Emit {

public:
  virtual ~Expr() = default;
  virtual void emit_impl(std::ostream &SS) = 0;
};

class RawExpr : public Expr {
  std::string Val;

public:
  RawExpr(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class VariableExpr : public Expr {
  VarDecl *D;

public:
  VariableExpr(VarDecl *D) : D(D) {}
  VarDecl *get_decl() { return D; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class SubscriptExpr : public Expr {
  Expr *Array;
  Expr *Index;

public:
  SubscriptExpr(Expr *Array, Expr *Index) : Array(Array), Index(Index) {}
  Expr *get_array() { return Array; }
  Expr *get_index() { return Index; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class CallExpr : public Expr {
  Expr *Callee;
  std::vector<Expr *> Args;

public:
  CallExpr(Expr *Callee, std::vector<Expr *> Args)
      : Callee(Callee), Args(Args) {}
  Expr *get_callee() { return Callee; }
  std::vector<Expr *> get_args() { return Args; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class UnaryOp : public Expr {
  Expr *Operand;
  std::string Op;
  bool IsPrefix;

public:
  UnaryOp(std::string Op, Expr *Operand, bool IsPrefix)
      : Operand(Operand), Op(Op), IsPrefix(IsPrefix) {}
  Expr *get_operand() { return Operand; }
  std::string get_op() { return Op; }
  bool is_prefix() { return IsPrefix; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class BinaryOp : public Expr {
  Expr *LHS;
  Expr *RHS;
  std::string Op;

public:
  BinaryOp(std::string Op, Expr *LHS, Expr *RHS) : LHS(LHS), RHS(RHS), Op(Op) {}
  Expr *get_lhs() { return LHS; }
  Expr *get_rhs() { return RHS; }
  std::string get_op() { return Op; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TernaryOp : public Expr {
  Expr *Cond;
  Expr *Then;
  Expr *Else;

public:
  TernaryOp(Expr *Cond, Expr *Then, Expr *Else)
      : Cond(Cond), Then(Then), Else(Else) {}
  Expr *get_cond() { return Cond; }
  Expr *get_then() { return Then; }
  Expr *get_else() { return Else; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class CastExpr : public Expr {
  Type *Ty;
  Expr *Operand;

public:
  CastExpr(Type *Ty, Expr *Operand) : Ty(Ty), Operand(Operand) {}
  Type *get_type() { return Ty; }
  Expr *get_operand() { return Operand; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ParenExpr : public Expr {
  Expr *Inside;

public:
  ParenExpr(Expr *Inside) : Inside(Inside) {}
  Expr *get_inside() { return Inside; }

protected:
  void emit_impl(std::ostream &SS) override;
};

// TODO: Initializer list.

} // namespace namec

#endif // NAMEC_GEN_EXPRS_H
