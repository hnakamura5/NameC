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
/**
  @brief CXXFile TopLevel. This can have preprocessor directives and namespace,
  class, other types and function definitions. One CXXFile can have multiple
  TopLevel in #if branches, and even you can split anywhere.
 */
class TopLevel : public Emit,
                 public DirectiveDefineMixin,
                 public UbiquitousDeclStmtMixin,
                 public ClassMemberDeclMixin {
  Context &C;
  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Namespace>> Namespaces;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  void on_add_class_member_decl(Decl *D) override { Entries.push_back(D); }
  void on_add_class_member_stmt_decl(Stmt *D) override { Entries.push_back(D); }

public:
  TopLevel(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixin(C),
        ClassMemberDeclMixin(C) {}
  // Only in top level we can define/declare functions
  FuncDecl *def_func(QualName Name, Type *RetTy, std::vector<VarDecl *> Params,
                     bool IsVarArg = false);
  FuncSplitDecl *def_func_declare(QualName Name, Type *RetTy,
                                  std::vector<VarDecl *> Params,
                                  bool IsVarArg = false);
  void def_func_define(FuncSplitDecl *Decl);
  FuncTemplateDecl *def_func_template(QualName Name,
                                      std::vector<VarDecl *> TemplateParams,
                                      Type *RetTy,
                                      std::vector<VarDecl *> Params,
                                      bool IsVarArg = false);
  TopLevel *def_namespace(QualName Name);
  UsingNamespaceStmt *stmt_using_namespace(QualName Name);

  // TopLevel Member definition API
  void def_method_define(MethodSplitDecl *Decl);
  CtorDecl *def_ctor(QualName ClassName, std::vector<VarDecl *> Params,
                     bool IsVarArgs = false);
  void def_ctor_define(CtorSplitDecl *CD);
  MethodDecl *def_dtor(QualName ClassName);
  void def_dtor_define(MethodSplitDecl *MD);

protected:
  void emit_impl(std::ostream &SS) override;
};

/**
  @brief FuncScope is for normal scope, such as function body, if body, etc.
  This can have directives, type declarations, statements.
 */
class FuncScope : public Emit,
                  public DirectiveDefineMixin,
                  public InFunctionStmtMixin,
                  public UbiquitousDeclStmtMixin {
  Context &C;

  std::vector<Emit *> Entries;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  FuncScope(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixin(C),
        InFunctionStmtMixin(C) {}
  UsingNamespaceStmt *stmt_using_namespace(QualName Name);
  virtual ~FuncScope() {}

protected:
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

protected:
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  MacroFuncScope(Context &C)
      : C(C), UbiquitousDeclStmtMixin(C), InFunctionStmtMixin(C) {}
  virtual ~MacroFuncScope() {}

protected:
  void emit_impl(std::ostream &SS) override;
};

/**
  @brief ClassTopLevel is toplevel in class/struct/union definition. This can
  have directives, type declarations, member definitions.
 */
class ClassTopLevel : public Emit,
                      public DirectiveDefineMixin,
                      public UbiquitousDeclStmtMixin,
                      public ClassMemberDeclMixin {
  Context &C;
  ClassOrUnion *Cls;

  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Namespace>> Namespaces;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  void on_add_class_member_decl(Decl *D) override { Entries.push_back(D); }
  void on_add_class_member_stmt_decl(Stmt *D) override { Entries.push_back(D); }

public:
  ClassTopLevel(Context &C, ClassOrUnion *Cls)
      : C(C), Cls(Cls), DirectiveDefineMixin(C), UbiquitousDeclStmtMixin(C),
        ClassMemberDeclMixin(C) {}
  virtual ~ClassTopLevel() {}

  MethodSplitDecl *def_method_declare(QualName Name, Type *RetTy,
                                      std::vector<VarDecl *> Params,
                                      bool IsVarArgs = false);
  CtorDecl *def_ctor(std::vector<VarDecl *> Params, bool IsVarArgs = false);
  CtorSplitDecl *def_ctor_declare(std::vector<VarDecl *> Params,
                                  bool IsVarArgs = false);
  MethodDecl *def_dtor();
  MethodSplitDecl *def_dtor_declare();

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_SCOPE_H
#undef NAMEC_GEN_SCOPE_H_CYCLIC
