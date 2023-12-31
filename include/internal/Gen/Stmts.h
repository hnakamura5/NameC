#ifndef NAMEC_GEN_STMTS_H
#define NAMEC_GEN_STMTS_H

#include "internal/Gen/Context.h"
#include "internal/Gen/Decl.h"
#include "internal/Gen/Forwards.h"

namespace namec {
class Stmt : public Emit {
protected:
  Context &C;

public:
  Stmt(Context &C) : C(C) {}
  virtual ~Stmt() = default;
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
  Expr *Cond;
  FuncScope *Then;
  std::vector<std::pair<Expr *, FuncScope *>> Elseifs;
  FuncScope *Else = nullptr;

public:
  IfStmt(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Then(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  FuncScope *get_then() { return Then; }
  FuncScope *add_elseif(Expr *Cond) {
    auto S = C.add_scope();
    Elseifs.push_back({Cond, S});
    return S;
  }
  FuncScope *get_or_add_else() {
    if (!Else) {
      Else = C.add_scope();
    }
    return Else;
  }
  bool has_else() { return Else != nullptr; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class WhileStmt : public Stmt {
  Expr *Cond;
  FuncScope *Body;

public:
  WhileStmt(Context &C, Expr *Cond)
      : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
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
      : Stmt(C), Cond(Cond), Step(Step), Body(C.add_scope()) {
    this->Init = std::move(Init);
  }
  Stmt *get_init() { return Init.get(); }
  Expr *get_cond() { return Cond; }
  Expr *get_step() { return Step; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class DoStmt : public Stmt {
  Expr *Cond;
  FuncScope *Body;

public:
  DoStmt(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class BlockStmt : public Stmt {
  FuncScope *S;

public:
  BlockStmt(Context &C) : Stmt(C), S(C.add_scope()) {}
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
  FuncScope *Body;
  bool IsFallThrough;

public:
  CaseStmt(Context &C, Expr *Val = nullptr, bool IsFallThrough = false)
      : Stmt(C), Val(Val), IsFallThrough(IsFallThrough), Body(C.add_scope()) {}
  Expr *get_val() { return Val; }
  bool is_default() { return Val == nullptr; }
  bool is_fall_through() { return IsFallThrough; }
  FuncScope *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class SwitchStmt : public Stmt {
  Expr *Cond;
  std::vector<std::unique_ptr<CaseStmt>> Cases;

public:
  SwitchStmt(Context &C, Expr *Cond) : Stmt(C), Cond(Cond) {}
  Expr *get_cond() { return Cond; }
  CaseStmt *add_case(Expr *Val, bool IsFallThrough = false);
  CaseStmt *add_default(bool IsFallThrough = false);

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_STMTS_H
