#ifndef NAMEC_GEN_STMTS_H
#define NAMEC_GEN_STMTS_H

#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Decl.h"

namespace namec {

class Stmt : public CommentMixin {

public:
  virtual ~Stmt() = default;
  virtual void emit(std::stringstream &SS) = 0;
};

class RawStmt : public Stmt {
  std::string Val;

public:
  RawStmt(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }
  void emit(std::stringstream &SS) override;
};

class VarDeclStmt : public Stmt {
  VarDecl *D;

public:
  VarDeclStmt(VarDecl *D) : D(D) {}
  VarDecl *get_decl() { return D; }
  void emit(std::stringstream &SS) override;
};

class IfStmt : public Stmt {
  Expr *Cond;
  Stmt *Then;
  Stmt *Else = nullptr;

public:
  IfStmt(Expr *Cond) : Cond(Cond) {}
  Expr *get_cond() { return Cond; }
  Scope *get_then();
  Scope *get_or_add_then();
  void emit(std::stringstream &SS) override;
};

class WhileStmt : public Stmt {
  Expr *Cond;
  Stmt *Body;

public:
  WhileStmt(Expr *Cond) : Cond(Cond) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

class ForStmt : public Stmt {
  Stmt *Init;
  Expr *Cond;
  Stmt *Step;
  Stmt *Body;

public:
  ForStmt(Stmt *Init, Expr *Cond, Stmt *Step)
      : Init(Init), Cond(Cond), Step(Step) {}
  Stmt *get_init() { return Init; }
  Expr *get_cond() { return Cond; }
  Stmt *get_step() { return Step; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

class DoStmt : public Stmt {
  Expr *Cond;
  Stmt *Body;

public:
  DoStmt(Expr *Cond) : Cond(Cond) {}
  Expr *get_cond() { return Cond; }
  Scope *get_body();
  void emit(std::stringstream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_STMTS_H
