#ifdef NAMEC_GENCXX_STMTS_H_CYCLIC
static_assert(false, "Cyclic include detected of " __FILE__);
#endif
#define NAMEC_GENCXX_STMTS_H_CYCLIC
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
  FuncScope *Then;
  std::vector<std::tuple<Expr *, FuncScope *, VarDecl *>> Elseifs;
  FuncScope *Else = nullptr;
  bool IsConstExpr;

public:
  IfStmt(Context &C, Expr *Cond, VarDecl *Init = nullptr,
         bool IsConstExpr = false)
      : Stmt(C), Cond(Cond), Then(C.add_func_scope()), Init(Init),
        IsConstExpr(IsConstExpr) {}
  Expr *get_cond() { return Cond; }
  VarDecl *get_init() { return Init; }
  FuncScope *get_then() { return Then; }
  FuncScope *add_elseif(Expr *Cond, VarDecl *Init = nullptr) {
    auto S = C.add_func_scope();
    Elseifs.push_back({Cond, S, Init});
    return S;
  }
  FuncScope *get_or_add_else() {
    if (!Else) {
      Else = C.add_func_scope();
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
  FuncScope *Body;

public:
  WhileStmt(Context &C, Expr *Cond)
      : Stmt(C), Cond(Cond), Body(C.add_func_scope()) {}
  Expr *get_cond() { return Cond; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ForStmt : public Stmt {
  std::unique_ptr<Stmt> Init;
  Expr *Cond;
  Expr *Step;
  FuncScope *Body;

public:
  ForStmt(Context &C, std::unique_ptr<Stmt> Init, Expr *Cond, Expr *Step)
      : Stmt(C), Cond(Cond), Step(Step), Body(C.add_func_scope()) {
    this->Init = std::move(Init);
  }
  Stmt *get_init() { return Init.get(); }
  Expr *get_cond() { return Cond; }
  Expr *get_step() { return Step; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ForRangeStmt : public Stmt {
  Decl *D;
  Expr *Range;
  FuncScope *Body;

public:
  ForRangeStmt(Context &C, Decl *D, Expr *Range)
      : Stmt(C), D(D), Range(Range), Body(C.add_func_scope()) {}
  Decl *get_decl() { return D; }
  Expr *get_range() { return Range; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class DoStmt : public Stmt {
  Expr *Cond;
  FuncScope *Body;

public:
  DoStmt(Context &C, Expr *Cond)
      : Stmt(C), Cond(Cond), Body(C.add_func_scope()) {}
  Expr *get_cond() { return Cond; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class BlockStmt : public Stmt {
  FuncScope *S;

public:
  BlockStmt(Context &C) : Stmt(C), S(C.add_func_scope()) {}
  FuncScope *get_scope() { return S; }

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
  std::string get_label_name() { return Name; }
  Stmt *get_stmt() { return S; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class GotoStmt : public Stmt {
  std::string Name;

public:
  GotoStmt(Context &C, std::string Name) : Stmt(C), Name(Name) {}
  std::string get_label_name() { return Name; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class CaseStmt : public Stmt {
  Expr *Val; //  nullptr for default
  FuncScope *Body;
  bool IsFallThrough;

public:
  CaseStmt(Context &C, Expr *Val = nullptr, bool IsFallThrough = false)
      : Stmt(C), Val(Val), IsFallThrough(IsFallThrough),
        Body(C.add_func_scope()) {}
  Expr *get_val() { return Val; }
  bool is_default() { return Val == nullptr; }
  bool is_fall_through() { return IsFallThrough; }
  FuncScope *get_body() { return Body; }

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
  Expr *E; // nullptr for rethrow

public:
  ThrowStmt(Context &C, Expr *E) : Stmt(C), E(E) {}
  Expr *get_expr() { return E; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TryStmt : public Stmt {
  FuncScope *Body;
  std::vector<std::pair<VarDecl *, FuncScope *>> Handlers;

public:
  TryStmt(Context &C) : Stmt(C), Body(C.add_func_scope()) {}
  FuncScope *get_body() { return Body; }
  // nullptr  for catch(...)
  FuncScope *add_handler(VarDecl *D = nullptr) {
    auto S = C.add_func_scope();
    Handlers.push_back({D, S});
    return S;
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_STMTS_H
#undef NAMEC_GENCXX_STMTS_H_CYCLIC
