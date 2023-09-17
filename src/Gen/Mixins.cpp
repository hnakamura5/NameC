#include "internal/Gen.h"

using namespace namec;

// Definitions for DirectiveDefineMixin methods
RawDirective *DirectiveDefineMixin::directive_raw(std::string Val) {
  return add(new RawDirective(Val));
}

Include *DirectiveDefineMixin::include(std::string Path) {
  return add(new Include(Path));
}

SystemInclude *DirectiveDefineMixin::include_sys(std::string Path) {
  return add(new SystemInclude(Path));
}

Define *DirectiveDefineMixin::def_macro_flag(std::string Name) {
  return add(new Define(Name, ""));
}

Define *DirectiveDefineMixin::def_macro_value(std::string Name,
                                              std::string Value) {
  return add(new Define(Name, Value));
}

Undef *DirectiveDefineMixin::def_macro_undef(std::string Name) {
  return add(new Undef(Name));
}

Pragma *DirectiveDefineMixin::pragma(std::string Value) {
  return add(new Pragma(Value));
}

RawDirective *DirectiveDefineMixin::insert_newline(size_t Count) {
  return directive_raw(std::string(Count, '\n'));
}

RawDirective *DirectiveDefineMixin::insert_space(size_t Count) {
  return directive_raw(std::string(Count, ' '));
}

RawDirective *DirectiveDefineMixin::insert_tab(size_t Count) {
  return directive_raw(std::string(Count, '\t'));
}

RawDirective *DirectiveDefineMixin::insert_comment(std::string Comment) {
  return directive_raw("/* " + Comment + " */");
}

RawDirective *DirectiveDefineMixin::insert_line_comment(std::string Comment) {
  return directive_raw("// " + Comment + "\n");
}

// Definitions for UbiquitousDeclStmtMixIn methods
RawDecl *UbiquitousDeclStmtMixIn::def_raw(std::string Val) {
  return add(C.decl_raw(Val));
}

VarDecl *UbiquitousDeclStmtMixIn::def_var(std::string Name, Type *T,
                                          Expr *Init) {
  return add(C.decl_var(Name, T, Init));
}

ArrayVarDecl *UbiquitousDeclStmtMixIn::def_array_var(std::string Name, Type *T,
                                                     std::vector<Expr *> Size,
                                                     Expr *Init) {
  return add(C.decl_array_var(Name, T, Size, Init));
}

StructDecl *UbiquitousDeclStmtMixIn::def_struct(std::string Name) {
  return add(C.decl_struct(C.type_struct(Name)));
}

UnionDecl *UbiquitousDeclStmtMixIn::def_union(std::string Name) {
  return add(C.decl_union(C.type_union(Name)));
}

EnumDecl *UbiquitousDeclStmtMixIn::def_enum(std::string Name) {
  return add(C.decl_enum(C.type_enum(Name)));
}

TypedefDecl *UbiquitousDeclStmtMixIn::def_typedef(std::string Name, Type *T) {
  return add(C.decl_typedef(Name, T));
}
