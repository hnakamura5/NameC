#ifndef NAMEC_GENCXX_EXPRS_H
#define NAMEC_GENCXX_EXPRS_H

#include "internal/GenCXX/CXXCommon.h"
#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {

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

class InitListExpr : public Expr {
  std::vector<Expr *> Values;

public:
  using iterator = decltype(Values)::iterator;
  InitListExpr(std::vector<Expr *> Values) : Values(Values) {}
  IteratorRange<iterator> values() {
    return IteratorRange<iterator>(Values.begin(), Values.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class NewExpr : public Expr {
  Type *Ty;
  std::vector<Expr *> Args;

public:
  using iterator = decltype(Args)::iterator;
  NewExpr(Type *Ty, std::vector<Expr *> Args) : Ty(Ty), Args(Args) {}
  Type *get_type() { return Ty; }
  IteratorRange<iterator> args() {
    return IteratorRange<iterator>(Args.begin(), Args.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class DeleteExpr : public Expr {
  Expr *E;

public:
  DeleteExpr(Expr *E) : E(E) {}
  Expr *get_expr() { return E; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class UserDefinedLiteral : public Expr {
  std::string Postfix;
  Expr *E;

public:
  UserDefinedLiteral(std::string Postfix, Expr *E) : Postfix(Postfix), E(E) {}
  std::string get_postfix() { return Postfix; }
  Expr *get_expr() { return E; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class InstantiateExpr : public Expr {
  TemplateDecl *TD;
  std::vector<Emit *> Args;

public:
  using iterator = decltype(Args)::iterator;
  InstantiateExpr(TemplateDecl *TD, std::vector<Emit *> Args)
      : TD(TD), Args(Args) {}
  TemplateDecl *get_template_decl() { return TD; }
  IteratorRange<iterator> args() {
    return IteratorRange<iterator>(Args.begin(), Args.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class QualNameExpr : public Expr {
  QualName *QN;

public:
  using iterator = QualName::iterator;
  QualNameExpr(std::vector<std::string> QN) : QN(QN) {}
  IteratorRange<iterator> names() { return QN->names(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

class LambdaExpr : public Expr {
  bool CaptureDefaultByCopy;
  std::vector<Expr *> Captures;
  std::vector<VarDecl *> Params;
  std::unique_ptr<Scope *> Body;
  // In general exprs cannot have attributes, but lambdas can.
  std::vector<Attribute *> Attrs;
  Type *RetTy;

public:
  using capture_iterator = decltype(Captures)::iterator;
  using param_iterator = decltype(Params)::iterator;
  LambdaExpr(Context &C, bool CaptureDefaultByCopy,
             std::vector<Expr *> Captures, std::vector<VarDecl *> Params,
             Type *RetTy = nullptr)
      : CaptureDefaultByCopy(CaptureDefaultByCopy), Captures(Captures),
        Params(Params), Body(C.add_scope()), Attrs(Attrs), RetTy(RetTy) {}
  bool capture_default_by_copy() { return CaptureDefaultByCopy; }
  IteratorRange<capture_iterator> captures() {
    return IteratorRange<capture_iterator>(Captures.begin(), Captures.end());
  }
  IteratorRange<param_iterator> params() {
    return IteratorRange<param_iterator>(Params.begin(), Params.end());
  }
  size_t size() { return Params.size(); }
  Scope *get_body() { return Body; }
  IteratorRange<decltype(Attrs)::iterator> attrs() {
    return IteratorRange<decltype(Attrs)::iterator>(Attrs.begin(), Attrs.end());
  }
  Type *get_ret_type() { return RetTy; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class FoldExpr : public Expr {
  std::string Op;
  Expr *Pack;
  // null means without initializer
  Expr *Init;
  bool IsLeftFold;

public:
  FoldExpr(std::string Op, Expr *Pack, Expr *Init, bool IsLeftFold)
      : Op(Op), Pack(Pack), Init(Init), IsLeftFold(IsLeftFold) {}
  std::string get_op() { return Op; }
  Expr *get_pack() { return Pack; }
  Expr *get_init() { return Init; }
  bool is_left_fold() { return IsLeftFold; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class RequiresExpr : public Expr {
  std::vector<Expr *> Params;
  std::vector<Stmt *> Requirements;

public:
  using param_iterator = decltype(Params)::iterator;
  using requirement_iterator = decltype(Requirements)::iterator;
  RequiresExpr(std::vector<Expr *> Params, std::vector<Stmt *> Requirements)
      : Params(Params) {}
  void add_requirement(Stmt *S) { Requirements.push_back(S); }
  IteratorRange<param_iterator> params() {
    return IteratorRange<param_iterator>(Params.begin(), Params.end());
  }
  IteratorRange<requirement_iterator> requirements() {
    return IteratorRange<requirement_iterator>(Requirements.begin(),
                                               Requirements.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_EXPRS_H
