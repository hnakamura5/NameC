#ifndef NAMEC_GEN_SCOPE_H
#define NAMEC_GEN_SCOPE_H

#include "internal/Gen/Directive.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/File.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/Mixins.h"
#include "internal/Gen/Stmts.h"

namespace namec {
/**
  @brief CFile TopLevel. This can have preprocessor directives and type,
  function definitions. One CFile can have multiple TopLevel in #if branches,
  and even you can split anywhere.
 */
class TopLevel : public Emit,
                 public DirectiveDefineMixin,
                 public UbiquitousDeclStmtMixin {
  Context &C;
  std::vector<Emit *> Entries;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  TopLevel(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixin(C) {}
  /// @brief Only in top level we can define/declare functions
  FuncDecl *def_func(std::string Name, Type *RetTy,
                     std::vector<VarDecl *> Params, bool IsVarArg = false);
  /// @brief This is declaration only, no definition.
  FuncSplitDecl *def_func_declare(std::string Name, Type *RetTy,
                                  std::vector<VarDecl *> Params,
                                  bool IsVarArg = false);
  /// @brief The definition of the function corresponding to the declaration.
  void def_func_define(FuncSplitDecl *Decl);

protected:
  void emit_impl(std::ostream &SS) override;
};

/**
  @brief FuncScope is for normal scope, such as function body, if body, etc.
  This can have directives, type declarations, statements.
 */
class FuncScope : public Emit,
                  public DirectiveDefineMixin,
                  public UbiquitousDeclStmtMixin,
                  public InFunctionStmtMixin {
  Context &C;

  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Stmt>> Stmts;

protected:
  virtual void on_add_directive(Directive *D) override { Entries.push_back(D); }
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  virtual void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  FuncScope(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixin(C),
        InFunctionStmtMixin(C) {}
  virtual ~FuncScope() {}
  void emit_impl(std::ostream &SS) override;
};

/**
  @brief MacroFuncScope is only as the toplevel scope of a macro function. Here
  directives are not allowed.
 */
class MacroFuncScope : public Emit,
                       public UbiquitousDeclStmtMixin,
                       public InFunctionStmtMixin {
  Context &C;
  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Stmt>> Stmts;

protected:
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  virtual void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  MacroFuncScope(Context &C)
      : C(C), UbiquitousDeclStmtMixin(C), InFunctionStmtMixin(C) {}
  virtual ~MacroFuncScope() {}
  void emit_impl(std::ostream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_SCOPE_H
