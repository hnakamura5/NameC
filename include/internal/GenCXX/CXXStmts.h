#ifndef NAMEC_GENCXX_STMTS_H
#define NAMEC_GENCXX_STMTS_H

#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {
class Stmt : public Emit {
protected:
  Context &C;
  std::vector<Attribute *> Attrs;
  virtual void emit_impl(std::ostream &SS) = 0;

public:
  Stmt(Context &C) : C(C) {}
  virtual ~Stmt() = default;
  void add_attr(Attribute *Attr) { Attrs.push_back(Attr); }
};

class RawStmt : public Stmt {
  std::string Val;

public:
  RawStmt(Context &C, std::string Val) : Stmt(C), Val(Val) {}
  std::string get_val() { return Val; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class DeclStmt : public Stmt {
  Decl *D;

public:
  DeclStmt(Context &C, Decl *D) : Stmt(C), D(D) {}
  Decl *get_decl() { return D; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class IfStmt : public Stmt {
  VarDecl *Init;
  Expr *Cond;
  Scope *Then;
  std::vector<std::pair<Expr *, Scope *>> Elseifs;
  Scope *Else = nullptr;
  bool IsConstExpr;

public:
  IfStmt(Context &C, Expr *Cond, VarDecl *Init = nullptr,
         bool IsConstExpr = false)
      : Stmt(C), Cond(Cond), Then(C.add_scope()), Init(Init),
        IsConstExpr(IsConstExpr) {}
  Expr *get_cond() { return Cond; }
  VarDecl *get_init() { return Init; }
  Scope *get_then() { return Then; }
  Scope *add_elseif(Expr *Cond) {
    auto S = C.add_scope();
    Elseifs.push_back({Cond, S});
    return S;
  }
  Scope *get_or_add_else() {
    if (!Else) {
      Else = C.add_scope();
    }
    return Else;
  }
  bool has_else() { return Else != nullptr; }
  bool is_constexpr() { return IsConstExpr; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class WhileStmt : public Stmt {
  Expr *Cond;
  Scope *Body;

public:
  WhileStmt(Context &C, Expr *Cond)
      : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ForStmt : public Stmt {
  std::unique_ptr<Stmt> Init;
  Expr *Cond;
  Expr *Step;
  Scope *Body;

public:
  ForStmt(Context &C, std::unique_ptr<Stmt> Init, Expr *Cond, Expr *Step)
      : Stmt(C), Cond(Cond), Step(Step), Body(C.add_scope()) {
    this->Init = std::move(Init);
  }
  Stmt *get_init() { return Init.get(); }
  Expr *get_cond() { return Cond; }
  Expr *get_step() { return Step; }
  Scope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ForRangeStmt : public Stmt {
  Decl *D;
  Expr *Range;
  Scope *Body;

public:
  ForRangeStmt(Context &C, Decl *D, Expr *Range)
      : Stmt(C), D(D), Range(Range), Body(C.add_scope()) {}
  Decl *get_decl() { return D; }
  Expr *get_range() { return Range; }
  Scope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class DoStmt : public Stmt {
  Expr *Cond;
  Scope *Body;

public:
  DoStmt(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class BlockStmt : public Stmt {
  Scope *S;

public:
  BlockStmt(Context &C) : Stmt(C), S(C.add_scope()) {}
  Scope *get_scope() { return S; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ExprStmt : public Stmt {
  Expr *E;

public:
  ExprStmt(Context &C, Expr *E) : Stmt(C), E(E) {}
  Expr *get_expr() { return E; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ReturnStmt : public Stmt {
  Expr *E;

public:
  ReturnStmt(Context &C, Expr *E) : Stmt(C), E(E) {}
  Expr *get_expr() { return E; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class BreakStmt : public Stmt {
public:
  BreakStmt(Context &C) : Stmt(C) {}

protected:
  void emit_impl(std::ostream &SS) override;
};

class ContinueStmt : public Stmt {
public:
  ContinueStmt(Context &C) : Stmt(C) {}

protected:
  void emit_impl(std::ostream &SS) override;
};

class LabelStmt : public Stmt {
  std::string Name;
  Stmt *S;

public:
  LabelStmt(Context &C, std::string Name, Stmt *S = nullptr)
      : Stmt(C), Name(Name), S(S) {}
  std::string get_name() { return Name; }
  Stmt *get_stmt() { return S; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class GotoStmt : public Stmt {
  std::string Name;

public:
  GotoStmt(Context &C, std::string Name) : Stmt(C), Name(Name) {}
  std::string get_name() { return Name; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class CaseStmt : public Stmt {
  Expr *Val; //  nullptr for default
  Scope *Body;
  bool IsFallThrough;

public:
  CaseStmt(Context &C, Expr *Val = nullptr, bool IsFallThrough = false)
      : Stmt(C), Val(Val), IsFallThrough(IsFallThrough), Body(C.add_scope()) {}
  Expr *get_val() { return Val; }
  bool is_default() { return Val == nullptr; }
  bool is_fall_through() { return IsFallThrough; }
  Scope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class SwitchStmt : public Stmt {
  VarDecl *Init;
  Expr *Cond;
  std::vector<std::unique_ptr<CaseStmt>> Cases;

public:
  SwitchStmt(Context &C, Expr *Cond, VarDecl *Init = nullptr)
      : Stmt(C), Cond(Cond), Init(Init) {}
  Expr *get_cond() { return Cond; }
  VarDecl *get_init() { return Init; }
  CaseStmt *add_case(Expr *Val, bool IsFallThrough = false);
  CaseStmt *add_default(bool IsFallThrough = false);

protected:
  void emit_impl(std::ostream &SS) override;
};

class ThrowStmt : public Stmt {
  Expr *E;

public:
  ThrowStmt(Context &C, Expr *E) : Stmt(C), E(E) {}
  Expr *get_expr() { return E; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TryStmt : public Stmt {
  Scope *Body;
  std::vector<std::pair<VarDecl *, Scope *>> Handlers;

public:
  TryStmt(Context &C) : Stmt(C), Body(C.add_scope()) {}
  Scope *get_body() { return Body; }
  Scope *add_handler(VarDecl *D) {
    auto S = C.add_scope();
    Handlers.push_back({D, S});
    return S;
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_STMTS_H
