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

IfStmt *InFunctionStmtMixIn::stmt_if(Expr *Cond) {
  return add(new IfStmt(C, Cond));
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

SwitchStmt *InFunctionStmtMixIn::stmt_switch(Expr *Cond) {
  return add(new SwitchStmt(C, Cond));
}

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
