#ifndef NAMEC_GEN_SCOPE_H
#define NAMEC_GEN_SCOPE_H

#include "internal/Gen/Directive.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/File.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Stmts.h"

namespace namec {
class Scope : public Emit,
              public DirectiveDefineMixin,
              public UbiquitousDeclStmtMixIn {
  Context &C;

  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Stmt>> Stmts;

  template <typename T> T *add(T *S) {
    Stmts.push_back(std::unique_ptr<Stmt>(S));
    return S;
  }

protected:
  virtual void on_add_directive(Directive *D) override { Entries.push_back(D); }
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  Scope(Context &C) : C(C), UbiquitousDeclStmtMixIn(C) {}
  virtual ~Scope() {}
  virtual void emit(std::stringstream &SS);

  // Stmt APIs
  RawStmt *stmt_raw(std::string Val) { return add(new RawStmt(C, Val)); }
  DeclStmt *stmt_decl(Decl *D) { return add(new DeclStmt(C, D)); }
  If *stmt_if(Expr *Cond) { return add(new If(C, Cond)); }
  While *stmt_while(Expr *Cond) { return add(new While(C, Cond)); }
  For *stmt_for(Stmt *Init, Expr *Cond, Stmt *Step) {
    return add(new For(C, Init, Cond, Step));
  }
  Do *stmt_do(Expr *Cond) { return add(new Do(C, Cond)); }
  Block *stmt_block() { return add(new Block(C)); }
  ExprStmt *stmt_expr(Expr *E) { return add(new ExprStmt(C, E)); }
  Return *stmt_return(Expr *Val = nullptr) { return add(new Return(C, Val)); }
  Break *stmt_break() { return add(new Break(C)); }
  Continue *stmt_continue() { return add(new Continue(C)); }
  Label *stmt_label(std::string Name, Stmt *S) {
    return add(new Label(C, Name, S));
  }
  Goto *stmt_goto(std::string Name) { return add(new Goto(C, Name)); }
  Switch *stmt_switch(Expr *Cond) { return add(new Switch(C, Cond)); }
};

} // namespace namec

#endif // NAMEC_GEN_SCOPE_H
