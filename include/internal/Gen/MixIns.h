#ifndef NAMEC_GEN_MIXINS_H
#define NAMEC_GEN_MIXINS_H

#include "internal/Gen/Directive.h"
#include "internal/Gen/Forwards.h"

namespace namec {

class DirectiveDefineMixin {
  std::vector<std::unique_ptr<Directive>> Directives;

protected:
  virtual void on_add_directive(Directive *D) = 0;

public:
  DirectiveDefineMixin() {}
  virtual ~DirectiveDefineMixin() {}

  Include *include(std::string Path);
  SystemInclude *include_sys(std::string Path);
  Define *def_macro_flag(std::string Name);
  Define def_macro_value(std::string Name, std::string Value);
  // TODO: void def_macro_func
  Define *def_macro_undef(std::string Name);
};

class UbiquitousDeclStmtMixIn {
  Context &C;
  std::vector<std::unique_ptr<Stmt>> Stmts;

protected:
  virtual void on_add_decl_stmt(Stmt *S) = 0;

public:
  UbiquitousDeclStmtMixIn(Context &C) : C(C) {}
  virtual ~UbiquitousDeclStmtMixIn() {}

  RawDecl *def_raw(std::string Val);
  VarDecl *def_var(std::string Name, Type *T, Expr *Init = nullptr);
  ArrayVarDecl *def_array_var(std::string Name, Type *T,
                              std::vector<Expr *> Size);
  StructDecl *def_struct(std::string Name);
  UnionDecl *def_union(std::string Name);
  EnumDecl *def_enum(std::string Name);
  TypedefDecl *def_typedef(std::string Name, Type *T);
};

} // namespace namec

#endif // NAMEC_GEN_MIXINS_H
