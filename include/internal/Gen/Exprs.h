#ifndef NAMEC_GEN_EXPRS_H
#define NAMEC_GEN_EXPRS_H

#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Decl.h"

namespace namec {

class Expr : public CommentMixin {

public:
  virtual ~Expr() = default;
  virtual void emit(std::stringstream &SS) = 0;
};

class RawExpr : public Expr {
  std::string Val;

public:
  RawExpr(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }
  void emit(std::stringstream &SS) override;
};

class Variable : public Expr {
  VarDecl *D;

public:
  Variable(VarDecl *D) : D(D) {}
  VarDecl *get_decl() { return D; }
  void emit(std::stringstream &SS) override;
};

class Subscript : public Expr {
  Expr *Array;
  Expr *Index;

public:
  Subscript(Expr *Array, Expr *Index) : Array(Array), Index(Index) {}
  Expr *get_array() { return Array; }
  Expr *get_index() { return Index; }
  void emit(std::stringstream &SS) override;
};

class Call : public Expr {
  Expr *Callee;
  std::vector<Expr *> Args;

public:
  Call(Expr *Callee, std::vector<Expr *> Args) : Callee(Callee), Args(Args) {}
  Expr *get_callee() { return Callee; }
  std::vector<Expr *> get_args() { return Args; }
  void emit(std::stringstream &SS) override;
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
  void emit(std::stringstream &SS) override;
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
  void emit(std::stringstream &SS) override;
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
  void emit(std::stringstream &SS) override;
};

class Cast : public Expr {
  Type *Ty;
  Expr *Operand;

public:
  Cast(Type *Ty, Expr *Operand) : Ty(Ty), Operand(Operand) {}
  Type *get_type() { return Ty; }
  Expr *get_operand() { return Operand; }
  void emit(std::stringstream &SS) override;
};

class Paren : public Expr {
  Expr *Inside;

public:
  Paren(Expr *Inside) : Inside(Inside) {}
  Expr *get_inside() { return Inside; }
  void emit(std::stringstream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_EXPRS_H
