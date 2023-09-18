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
    Entries.push_back(S);
    return S;
  }

protected:
  virtual void on_add_directive(Directive *D) override { Entries.push_back(D); }
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  Scope(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixIn(C) {}
  virtual ~Scope() {}
  void emit_impl(std::ostream &SS) override;

  // Stmt APIs
  RawStmt *stmt_raw(std::string Val) { return add(new RawStmt(C, Val)); }
  RawStmt *stmt_empty() { return stmt_raw(";"); }
  DeclStmt *stmt_decl(Decl *D) { return add(new DeclStmt(C, D)); }
  IfStmt *stmt_if(Expr *Cond) { return add(new IfStmt(C, Cond)); }
  WhileStmt *stmt_while(Expr *Cond) { return add(new WhileStmt(C, Cond)); }
  ForStmt *stmt_for(VarDecl *Init, Expr *Cond, Expr *Step);
  ForStmt *stmt_for(Expr *Init, Expr *Cond, Expr *Step);
  DoStmt *stmt_do(Expr *Cond) { return add(new DoStmt(C, Cond)); }
  BlockStmt *stmt_block() { return add(new BlockStmt(C)); }
  ExprStmt *stmt_expr(Expr *E) { return add(new ExprStmt(C, E)); }
  ReturnStmt *stmt_return(Expr *Val = nullptr) {
    return add(new ReturnStmt(C, Val));
  }
  BreakStmt *stmt_break() { return add(new BreakStmt(C)); }
  ContinueStmt *stmt_continue() { return add(new ContinueStmt(C)); }
  LabelStmt *stmt_label(std::string Name, Stmt *S = nullptr) {
    return add(new LabelStmt(C, Name, S));
  }
  GotoStmt *stmt_goto(std::string Name) { return add(new GotoStmt(C, Name)); }
  SwitchStmt *stmt_switch(Expr *Cond) { return add(new SwitchStmt(C, Cond)); }
};

} // namespace namec

#endif // NAMEC_GEN_SCOPE_H
