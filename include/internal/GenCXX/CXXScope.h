#ifdef NAMEC_GENCXX_SCOPE_H_CYCLIC
static_assert(false, "Cyclic include detected of " __FILE__);
#endif
#define NAMEC_GENCXX_SCOPE_H_CYCLIC

#ifndef NAMEC_GENCXX_SCOPE_H
#define NAMEC_GENCXX_SCOPE_H

#include "internal/GenCXX/CXXCommon.h"
#include "internal/GenCXX/CXXForwards.h"
#include "internal/GenCXX/CXXMixins.h"

namespace namecxx {

class TopLevel : public Emit,
                 public DirectiveDefineMixin,
                 public UbiquitousDeclStmtMixIn {
  Context &C;
  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Namespace>> Namespaces;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  TopLevel(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixIn(C) {}
  // Only in top level we can define/declare functions
  FuncDecl *def_func(QualName Name, Type *RetTy, std::vector<VarDecl *> Params,
                     bool IsVarArg = false);
  TopLevel *def_namespace(QualName Name);

protected:
  void emit_impl(std::ostream &SS) override;
};

class FuncScope : public Emit,
                  public DirectiveDefineMixin,
                  public InFunctionStmtMixIn,
                  public UbiquitousDeclStmtMixIn {
  Context &C;

  std::vector<Emit *> Entries;

protected:
  virtual void on_add_directive(Directive *D) override { Entries.push_back(D); }
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  virtual void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  FuncScope(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixIn(C),
        InFunctionStmtMixIn(C) {}
  virtual ~FuncScope() {}

protected:
  void emit_impl(std::ostream &SS) override;
};

class MacroFuncScope : public Emit,
                       public UbiquitousDeclStmtMixIn,
                       public InFunctionStmtMixIn {
  Context &C;
  std::vector<Emit *> Entries;

protected:
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  virtual void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  MacroFuncScope(Context &C)
      : C(C), UbiquitousDeclStmtMixIn(C), InFunctionStmtMixIn(C) {}
  virtual ~MacroFuncScope() {}

protected:
  void emit_impl(std::ostream &SS) override;
};

class ClassTopLevel : public Emit,
                      public DirectiveDefineMixin,
                      public UbiquitousDeclStmtMixIn {
  Context &C;

  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Namespace>> Namespaces;

protected:
  virtual void on_add_directive(Directive *D) override { Entries.push_back(D); }
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  ClassTopLevel(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixIn(C) {}
  virtual ~ClassTopLevel() {}

  TopLevel *def_namespace(QualName Name);
  VarDecl *def_field(QualName Name, Type *T, Expr *Init = nullptr);
  VarTemplateDecl *def_field_template(QualName Name,
                                      std::vector<VarDecl *> TemplateParams,
                                      Type *T, std::vector<Expr *> Args,
                                      Expr *Init = nullptr);
  MethodDecl *def_method(QualName Name, std::vector<VarDecl *> Params,
                         Type *RetTy, bool IsVarArgs = false);
  MethodTemplateDecl *def_method_template(QualName Name,
                                          std::vector<VarDecl *> TemplateParams,
                                          Type *RetTy,
                                          std::vector<VarDecl *> Params,
                                          bool IsVarArgs = false);
  MethodDecl *def_ctor(std::vector<VarDecl *> Params, bool IsVarArgs = false);
  MethodDecl *def_dtor();

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_SCOPE_H
#undef NAMEC_GEN_SCOPE_H_CYCLIC
