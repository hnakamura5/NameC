#ifndef NAMEC_GEN_STMTS_H
#define NAMEC_GEN_STMTS_H

#include "internal/Gen/Context.h"
#include "internal/Gen/Decl.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"

namespace namec {
// To eliminate circular dependency of Scope constructor

class Stmt : public Emit {
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
  void emit(std::stringstream &SS) override;
};

class DeclStmt : public Stmt {
  Decl *D;

public:
  DeclStmt(Context &C, Decl *D) : Stmt(C), D(D) {}
  Decl *get_decl() { return D; }
  void emit(std::stringstream &SS) override;
};

class If : public Stmt {
  Expr *Cond;
  std::unique_ptr<Scope> Then;
  std::unique_ptr<Scope> Else = nullptr;

public:
  If(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Then(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  Scope *get_then();
  Scope *get_or_add_else();
  void emit(std::stringstream &SS) override;
};

class While : public Stmt {
  Expr *Cond;
  std::unique_ptr<Scope> Body;

public:
  While(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

class For : public Stmt {
  Stmt *Init;
  Expr *Cond;
  Stmt *Step;
  std::unique_ptr<Scope> Body;

public:
  For(Context &C, Stmt *Init, Expr *Cond, Stmt *Step)
      : Init(Init), Cond(Cond), Step(Step), Body(C.add_scope()) {}
  Stmt *get_init() { return Init; }
  Expr *get_cond() { return Cond; }
  Stmt *get_step() { return Step; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

class Do : public Stmt {
  Expr *Cond;
  std::unique_ptr<Scope> Body;

public:
  Do(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

class Block : public Stmt {
  std::unique_ptr<Scope> S;

public:
  Block(Context &C) : Stmt(C), S(C.add_scope()) {}
  Scope *get_scope() { return S.get(); }
  void emit(std::stringstream &SS) override;
};

class ExprStmt : public Stmt {
  Expr *E;

public:
  ExprStmt(Context &C, Expr *E) : Stmt(C), E(E) {}
  Expr *get_expr() { return E; }
  void emit(std::stringstream &SS) override;
};

class Return : public Stmt {
  Expr *E;

public:
  Return(Context &C, Expr *E) : Stmt(C), E(E) {}
  Expr *get_expr() { return E; }
  void emit(std::stringstream &SS) override;
};

class Break : public Stmt {
public:
  Break(Context &C) : Stmt(C) {}
  void emit(std::stringstream &SS) override;
};

class Continue : public Stmt {
public:
  Continue(Context &C) : Stmt(C) {}
  void emit(std::stringstream &SS) override;
};

class Label : public Stmt {
  std::string Name;
  Stmt *S;

public:
  Label(Context &C, std::string Name, Stmt *S) : Stmt(C), Name(Name), S(S) {}
  std::string get_name() { return Name; }
  Stmt *get_stmt() { return S; }
  void emit(std::stringstream &SS) override;
};

class Goto : public Stmt {
  std::string Name;

public:
  Goto(Context &C, std::string Name) : Stmt(C), Name(Name) {}
  std::string get_name() { return Name; }
  void emit(std::stringstream &SS) override;
};

class Switch : public Stmt {
  Expr *Cond;
  std::unique_ptr<Scope> Body;

public:
  Switch(Context &C, Expr *Cond) : Stmt(C), Cond(Cond), Body(C.add_scope()) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_STMTS_H
