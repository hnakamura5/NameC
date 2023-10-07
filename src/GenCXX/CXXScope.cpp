#include "internal/GenCXX.h"

using namespace namecxx;

FuncDecl *TopLevel::def_func(QualName Name, Type *RetTy,
                             std::vector<VarDecl *> Params, bool IsVarArg) {
  auto *F = C.decl_func(Name, RetTy, Params, IsVarArg);
  Entries.push_back(F);
  return F;
}

FuncTemplateDecl *
TopLevel::def_func_template(QualName Name,
                            std::vector<VarDecl *> TemplateParams, Type *RetTy,
                            std::vector<VarDecl *> Params, bool IsVarArg) {
  auto *F = C.decl_func_template(Name, TemplateParams, RetTy, Params, IsVarArg);
  Entries.push_back(F);
  return F;
}

TopLevel *TopLevel::def_namespace(QualName Name) {
  auto *NS = new Namespace(C, Name);
  Namespaces.push_back(std::unique_ptr<Namespace>(NS));
  Entries.push_back(NS);
  return NS->get_body();
}

UsingNamespaceStmt *TopLevel::stmt_using_namespace(QualName Name) {
  auto *D = new UsingNamespaceStmt(C, Name);
  Entries.push_back(D);
  return D;
}

void TopLevel::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    SS << E << "\n";
  }
}

UsingNamespaceStmt *FuncScope::stmt_using_namespace(QualName Name) {
  auto *D = new UsingNamespaceStmt(C, Name);
  Entries.push_back(D);
  return D;
}

void FuncScope::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    SS << E;
  }
}

void MacroFuncScope::emit_impl(std::ostream &SS) {
  std::stringstream LocalSS;
  for (auto *E : Entries) {
    LocalSS << E;
  }
  std::string LocalStr = LocalSS.str();
  // Replace all newline with newline in macro
  size_t Pos = 0;
  while ((Pos = LocalStr.find("\n", Pos)) != std::string::npos) {
    LocalStr.replace(Pos, 1, "\\\n");
    Pos += 2;
  }
  SS << LocalStr;
}

TopLevel *ClassTopLevel::def_namespace(QualName Name) {
  auto *NS = new Namespace(C, Name);
  Namespaces.push_back(std::unique_ptr<Namespace>(NS));
  Entries.push_back(NS);
  return NS->get_body();
}

VarDecl *ClassTopLevel::def_field(QualName Name, Type *T, Expr *Init) {
  auto *D = C.decl_var(Name, T, Init);
  Entries.push_back(add_stmt_decl(D));
  return D;
}

VarTemplateDecl *ClassTopLevel::def_field_template(
    QualName Name, std::vector<VarDecl *> TemplateParams, Type *T, Expr *Init) {
  auto *D = C.decl_var_template(Name, TemplateParams, T, Init);
  Entries.push_back(add_stmt_decl(D));
  return D;
}

MethodDecl *ClassTopLevel::def_method(QualName Name, Type *RetTy,
                                      std::vector<VarDecl *> Params,
                                      bool IsVarArgs) {
  auto *D = C.decl_method(Name, Cls, RetTy, Params, IsVarArgs);
  Entries.push_back(D);
  return D;
}

MethodTemplateDecl *ClassTopLevel::def_method_template(
    QualName Name, std::vector<VarDecl *> TemplateParams, Type *RetTy,
    std::vector<VarDecl *> Params, bool IsVarArgs) {
  auto *D = C.decl_method_template(Name, Cls, TemplateParams, RetTy, Params,
                                   IsVarArgs);
  Entries.push_back(D);
  return D;
}

CtorDecl *ClassTopLevel::def_ctor(std::vector<VarDecl *> Params,
                                  bool IsVarArgs) {
  auto *D = C.decl_ctor(Cls, Params, IsVarArgs);
  Entries.push_back(D);
  return D;
}

MethodDecl *ClassTopLevel::def_dtor() {
  // TODO: Dirty implementation
  auto *D =
      C.decl_method("~" + Cls->get_name().get_names().back(), Cls, nullptr, {});
  Entries.push_back(D);
  return D;
}

void ClassTopLevel::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    SS << E << "\n";
  }
}
