#include "internal/GenCXX.h"

using namespace namecxx;

FuncDecl *TopLevel::def_func(QualName Name, Type *RetTy,
                             std::vector<VarDecl *> Params, bool IsVarArg) {
  auto *F = C.decl_func(Name, RetTy, Params, IsVarArg);
  Entries.push_back(F);
  return F;
}

FuncSplitDecl *TopLevel::def_func_declare(QualName Name, Type *RetTy,
                                          std::vector<VarDecl *> Params,
                                          bool IsVarArg) {
  auto *FD = C.decl_func_split(Name, RetTy, Params, IsVarArg);
  auto *Fwd = C.decl_func_split_forward(FD);
  Entries.push_back(Fwd);
  return FD;
}

void TopLevel::def_func_define(FuncSplitDecl *D) { Entries.push_back(D); }

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

void TopLevel::def_method_define(MethodSplitDecl *Decl) {
  Entries.push_back(Decl);
}

CtorDecl *TopLevel::def_ctor(QualName ClassName, std::vector<VarDecl *> Params,
                             bool IsVarArgs) {
  auto *D = C.decl_ctor(ClassName.last(), Params, IsVarArgs);
  Entries.push_back(D);
  return D;
}

void TopLevel::def_ctor_define(CtorSplitDecl *CD) { Entries.push_back(CD); }

MethodDecl *TopLevel::def_dtor(QualName ClassName) {

  auto *D = C.decl_method("~" + ClassName.last(), nullptr, {});
  Entries.push_back(D);
  return D;
}

void TopLevel::def_dtor_define(MethodSplitDecl *MD) { Entries.push_back(MD); }

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

MethodSplitDecl *ClassTopLevel::def_method_declare(
    QualName Name, Type *RetTy, std::vector<VarDecl *> Params, bool IsVarArgs) {
  auto *MD = C.decl_method_split(Name, Cls, RetTy, Params, IsVarArgs);
  auto *Fwd = C.decl_method_split_forward(MD);
  Entries.push_back(Fwd);
  return MD;
}

CtorDecl *ClassTopLevel::def_ctor(std::vector<VarDecl *> Params,
                                  bool IsVarArgs) {
  auto *D = C.decl_ctor(Cls->get_name().last(), Params, IsVarArgs);
  Entries.push_back(D);
  return D;
}

CtorSplitDecl *ClassTopLevel::def_ctor_declare(std::vector<VarDecl *> Params,
                                               bool IsVarArgs) {
  auto *CD = C.decl_ctor_split(Cls, Params, IsVarArgs);
  auto *Fwd = C.decl_ctor_split_forward(CD);
  Entries.push_back(Fwd);
  return CD;
}

MethodDecl *ClassTopLevel::def_dtor() {
  // TODO: Dirty implementation
  auto *D = C.decl_method("~" + Cls->get_name().last(), nullptr, {});
  Entries.push_back(D);
  return D;
}

MethodSplitDecl *ClassTopLevel::def_dtor_declare() {
  auto *MD =
      C.decl_method_split("~" + Cls->get_name().last(), Cls, nullptr, {});
  auto *Fwd = C.decl_method_split_forward(MD);
  Entries.push_back(Fwd);
  return MD;
}

void ClassTopLevel::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    SS << E << "\n";
  }
}
