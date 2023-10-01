#include "internal/GenCXX.h"

using namespace namecxx;

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

DefineFuncMacro *
DirectiveDefineMixin::def_macro_func(std::string Name,
                                     std::vector<std::string> Params) {
  return add(new DefineFuncMacro(C, Name, Params));
}

Undef *DirectiveDefineMixin::def_macro_undef(std::string Name) {
  return add(new Undef(Name));
}

Pragma *DirectiveDefineMixin::pragma(std::string Value) {
  return add(new Pragma(Value));
}

IfDirective *DirectiveDefineMixin::directive_if(Expr *Cond) {
  return add(new IfDirective(C, Cond));
}

Ifdef *DirectiveDefineMixin::directive_ifdef(std::string Cond) {
  return add(new Ifdef(C, Cond));
}

Ifndef *DirectiveDefineMixin::directive_ifndef(std::string Cond) {
  return add(new Ifndef(C, Cond));
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

VarDecl *UbiquitousDeclStmtMixIn::def_var(QualName Name, Type *T, Expr *Init) {
  return add(C.decl_var(Name, T, Init));
}

VarTemplateDecl *UbiquitousDeclStmtMixIn::def_var_template(
    QualName Name, std::vector<VarDecl *> TemplateParams, Type *T, Expr *Init) {
  return add(C.decl_var_template(Name, TemplateParams, T, Init));
}

ArrayVarDecl *UbiquitousDeclStmtMixIn::def_array_var(QualName Name, Type *T,
                                                     std::vector<Expr *> Size,
                                                     Expr *Init) {
  return add(C.decl_array_var(Name, T, Size, Init));
}

ClassDecl *UbiquitousDeclStmtMixIn::def_struct(
    QualName Name, std::vector<std::pair<AccessSpec, Type *>> Bases) {
  return add(C.decl_class(C.type_class(Name, Bases, true)));
}

ClassDecl *UbiquitousDeclStmtMixIn::def_class(
    QualName Name, std::vector<std::pair<AccessSpec, Type *>> Bases) {
  return add(C.decl_class(C.type_class(Name, Bases, false)));
}

ClassTemplateDecl *UbiquitousDeclStmtMixIn::def_struct_template(
    QualName Name, std::vector<VarDecl *> TemplateParams,
    std::vector<std::pair<AccessSpec, Type *>> Bases) {
  return add(
      C.decl_class_template(C.type_class(Name, Bases, true), TemplateParams));
}

ClassTemplateDecl *UbiquitousDeclStmtMixIn::def_class_template(
    QualName Name, std::vector<VarDecl *> TemplateParams,
    std::vector<std::pair<AccessSpec, Type *>> Bases) {
  return add(
      C.decl_class_template(C.type_class(Name, Bases, false), TemplateParams));
}

UnionDecl *UbiquitousDeclStmtMixIn::def_union(QualName Name) {
  return add(C.decl_union(C.type_union(Name)));
}

UnionTemplateDecl *UbiquitousDeclStmtMixIn::def_union_template(
    QualName Name, std::vector<VarDecl *> TemplateParams) {
  return add(C.decl_union_template(C.type_union(Name), TemplateParams));
}

EnumDecl *UbiquitousDeclStmtMixIn::def_enum(QualName Name) {
  return add(C.decl_enum(C.type_enum(Name)));
}

TypedefDecl *UbiquitousDeclStmtMixIn::def_typedef(QualName Name, Type *T) {
  return add(C.decl_typedef(Name, T));
}

UsingDecl *UbiquitousDeclStmtMixIn::def_using(QualName Name, Type *T) {
  return add(C.decl_using(Name, T));
}
UsingTemplateDecl *UbiquitousDeclStmtMixIn::def_using_template(
    QualName Name, std::vector<VarDecl *> TemplateParams, Type *T) {
  return add(C.decl_using_template(Name, TemplateParams, T));
}

// Definitions for InFunctionStmtMixIn methods
RawStmt *InFunctionStmtMixIn::stmt_raw(std::string Val) {
  return add(new RawStmt(C, Val));
}

RawStmt *InFunctionStmtMixIn::stmt_empty() { return stmt_raw(";"); }

DeclStmt *InFunctionStmtMixIn::stmt_decl(Decl *D) {
  return add(new DeclStmt(C, D));
}

ExprStmt *InFunctionStmtMixIn::stmt_expr(Expr *E) {
  return add(new ExprStmt(C, E));
}

IfStmt *InFunctionStmtMixIn::stmt_if(Expr *Cond, VarDecl *Init,
                                     bool IsConstExpr) {
  return add(new IfStmt(C, Cond, Init, IsConstExpr));
}

WhileStmt *InFunctionStmtMixIn::stmt_while(Expr *Cond) {
  return add(new WhileStmt(C, Cond));
}

ForStmt *InFunctionStmtMixIn::stmt_for(VarDecl *Init, Expr *Cond, Expr *Step) {
  auto InitStmt = std::make_unique<DeclStmt>(C, Init);
  ForStmt *S = new ForStmt(C, std::move(InitStmt), Cond, Step);
  return add(S);
}

ForStmt *InFunctionStmtMixIn::stmt_for(Expr *Init, Expr *Cond, Expr *Step) {
  auto InitStmt = std::make_unique<ExprStmt>(C, Init);
  ForStmt *S = new ForStmt(C, std::move(InitStmt), Cond, Step);
  return add(S);
}

ForRangeStmt *InFunctionStmtMixIn::stmt_for_range(VarDecl *Init, Expr *Range) {
  return add(new ForRangeStmt(C, Init, Range));
}

DoStmt *InFunctionStmtMixIn::stmt_do(Expr *Cond) {
  return add(new DoStmt(C, Cond));
}

BlockStmt *InFunctionStmtMixIn::stmt_block() { return add(new BlockStmt(C)); }

ReturnStmt *InFunctionStmtMixIn::stmt_return(Expr *Val) {
  return add(new ReturnStmt(C, Val));
}

BreakStmt *InFunctionStmtMixIn::stmt_break() { return add(new BreakStmt(C)); }
ContinueStmt *InFunctionStmtMixIn::stmt_continue() {
  return add(new ContinueStmt(C));
}

LabelStmt *InFunctionStmtMixIn::stmt_label(std::string Name, Stmt *S) {
  return add(new LabelStmt(C, Name, S));
}

GotoStmt *InFunctionStmtMixIn::stmt_goto(std::string Name) {
  return add(new GotoStmt(C, Name));
}

SwitchStmt *InFunctionStmtMixIn::stmt_switch(Expr *Cond, VarDecl *Init) {
  return add(new SwitchStmt(C, Cond, Init));
}

ExprStmt *InFunctionStmtMixIn::stmt_delete(Expr *Val, bool IsArray) {
  return stmt_expr(C.expr_delete(Val, IsArray));
}

ThrowStmt *InFunctionStmtMixIn::stmt_throw(Expr *Val) {
  return add(new ThrowStmt(C, Val));
}

TryStmt *InFunctionStmtMixIn::stmt_try() { return add(new TryStmt(C)); }

ExprStmt *InFunctionStmtMixIn::stmt_assign(Expr *LHS, Expr *RHS) {
  return stmt_expr(C.expr_binary("=", LHS, RHS));
}

ExprStmt *InFunctionStmtMixIn::stmt_call(Expr *Callee,
                                         std::vector<Expr *> Args) {
  return stmt_expr(C.expr_call(Callee, Args));
}

VarDecl *InFunctionStmtMixIn::stmt_va_list(std::string Name) {
  auto *D = C.decl_var(Name, C.type_va_list());
  add(new DeclStmt(C, D));
  return D;
}

ExprStmt *InFunctionStmtMixIn::stmt_va_start(Expr *VaList, Expr *Count) {
  return stmt_call(C.expr_var_name("va_start"), {VaList, Count});
}

ExprStmt *InFunctionStmtMixIn::stmt_va_end(Expr *Arg) {
  return stmt_call(C.expr_var_name("va_end"), {Arg});
}

ExprStmt *InFunctionStmtMixIn::stmt_va_copy(Expr *Dest, Expr *Src) {
  return stmt_call(C.expr_var_name("va_copy"), {Dest, Src});
}
