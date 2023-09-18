#ifndef NAMEC_GEN_MIXINS_H
#define NAMEC_GEN_MIXINS_H

#include "internal/Gen/Decl.h"
#include "internal/Gen/Directive.h"
#include "internal/Gen/Exprs.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/Stmts.h"

namespace namec {

class DirectiveDefineMixin {
  Context &C;
  std::vector<std::unique_ptr<Directive>> Directives;

  template <typename T> T *add(T *D) {
    Directives.push_back(std::unique_ptr<Directive>(D));
    on_add_directive(D);
    return D;
  }

protected:
  virtual void on_add_directive(Directive *D) = 0;

public:
  DirectiveDefineMixin(Context &C) : C(C) {}
  virtual ~DirectiveDefineMixin() {}

  RawDirective *directive_raw(std::string Val);
  Include *include(std::string Path);
  SystemInclude *include_sys(std::string Path);
  Define *def_macro_flag(std::string Name);
  Define *def_macro_value(std::string Name, std::string Value);
  DefineFuncMacro *def_macro_func(std::string Name,
                                  std::vector<std::string> Params);
  // TODO: void def_macro_func
  Undef *def_macro_undef(std::string Name);
  Pragma *pragma(std::string Value);
  IfDirective *directive_if(Expr *Cond);
  Ifdef *directive_ifdef(std::string Cond);
  Ifndef *directive_ifndef(std::string Cond);

  // These semantically meaningless entities are defined as directive for
  // the convenience.
  // We does not strongly recommend to use them. We readied them for the case
  // of debugging. Usually, you should use code formatter after code
  // generation.
  RawDirective *insert_newline(size_t Count = 1);
  RawDirective *insert_space(size_t Count = 2);
  RawDirective *insert_tab(size_t Count = 1);
  RawDirective *insert_comment(std::string Comment);
  RawDirective *insert_line_comment(std::string Comment);
};

class UbiquitousDeclStmtMixIn {
  Context &C;
  std::vector<std::unique_ptr<Stmt>> Stmts;

  template <typename T> T *add(T *D) {
    auto *S = new DeclStmt(C, D);
    Stmts.push_back(std::unique_ptr<Stmt>(S));
    on_add_decl_stmt(S);
    return D;
  }

protected:
  virtual void on_add_decl_stmt(Stmt *S) = 0;

public:
  UbiquitousDeclStmtMixIn(Context &C) : C(C) {}
  virtual ~UbiquitousDeclStmtMixIn() {}

  RawDecl *def_raw(std::string Val);
  VarDecl *def_var(std::string Name, Type *T, Expr *Init = nullptr);
  ArrayVarDecl *def_array_var(std::string Name, Type *T,
                              std::vector<Expr *> Size, Expr *Init = nullptr);
  StructDecl *def_struct(std::string Name);
  UnionDecl *def_union(std::string Name);
  EnumDecl *def_enum(std::string Name);
  TypedefDecl *def_typedef(std::string Name, Type *T);
};

class InFunctionStmtMixIn {
  Context &C;
  std::vector<std::unique_ptr<Stmt>> Stmts;

  template <typename T> T *add(T *D) {
    Stmts.push_back(std::unique_ptr<Stmt>(D));
    on_add_stmt(D);
    return D;
  }

protected:
  virtual void on_add_stmt(Stmt *S) = 0;

public:
  InFunctionStmtMixIn(Context &C) : C(C) {}
  virtual ~InFunctionStmtMixIn() {}

  RawStmt *stmt_raw(std::string Val);
  RawStmt *stmt_empty();
  DeclStmt *stmt_decl(Decl *D);
  IfStmt *stmt_if(Expr *Cond);
  WhileStmt *stmt_while(Expr *Cond);
  ForStmt *stmt_for(VarDecl *Init, Expr *Cond, Expr *Step);
  ForStmt *stmt_for(Expr *Init, Expr *Cond, Expr *Step);
  DoStmt *stmt_do(Expr *Cond);
  BlockStmt *stmt_block();
  ExprStmt *stmt_expr(Expr *E);
  ReturnStmt *stmt_return(Expr *Val = nullptr);
  BreakStmt *stmt_break();
  ContinueStmt *stmt_continue();
  LabelStmt *stmt_label(std::string Name, Stmt *S = nullptr);
  GotoStmt *stmt_goto(std::string Name);
  SwitchStmt *stmt_switch(Expr *Cond);
};

} // namespace namec

#endif // NAMEC_GEN_MIXINS_H
