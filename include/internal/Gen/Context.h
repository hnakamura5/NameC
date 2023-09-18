#ifndef NAMEC_GEN_CONTEXT_H
#define NAMEC_GEN_CONTEXT_H

#include "internal/Gen/Decl.h"
#include "internal/Gen/Exprs.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Types.h"

namespace namec {

// Context manage Scope, Decl and Expr heap existence. Stmt are kept in Scope.
// This also exists to eliminate complex circular dependency between Scope,
// Decl, Stmt and Expr.
class Context {
  friend class File;
  friend class Scope;
  friend class TopLevel;
  friend class UbiquitousDeclStmtMixIn;

  std::vector<std::unique_ptr<Scope>> Scopes;
  std::vector<std::unique_ptr<Decl>> Decls;
  std::vector<std::unique_ptr<Expr>> Exprs;
  std::vector<std::unique_ptr<Type>> Types;
  std::unique_ptr<Void> VoidSingleton;
  std::map<std::string, RawType *> RawTypeMap;
  std::map<Type *, Pointer *> PointerTypeMap;
  std::map<Type *, Array *> ArrayTypeMap;
  std::map<Decl *, Named *> NamedTypeMap;

  template <typename T> T *add_decl(T *D) {
    Decls.push_back(std::unique_ptr<Decl>(D));
    return D;
  }

  template <typename T> T *add_expr(T *E) {
    Exprs.push_back(std::unique_ptr<Expr>(E));
    return E;
  }

  template <typename T> T *add_type(T *Ty) {
    Types.push_back(std::unique_ptr<Type>(Ty));
    return Ty;
  }

  RawType *get_or_add_raw_type(std::string Val) {
    if (auto P = RawTypeMap.find(Val); P != RawTypeMap.end()) {
      return P->second;
    }
    auto *Ty = add_type(new RawType(Val));
    RawTypeMap[Val] = Ty;
    return Ty;
  }

public:
  Context() { VoidSingleton.reset(new Void()); }
  ~Context() = default;

  Scope *add_scope();

private:
  // Decl factory APIs. Not public to user. Intended to be used by internal
  // construction in File and Scope.
  RawDecl *decl_raw(std::string Val) { return add_decl(new RawDecl(Val)); }
  ArrayVarDecl *decl_array_var(std::string Name, Type *T,
                               std::vector<Expr *> Size, Expr *Init = nullptr) {
    return add_decl(new ArrayVarDecl(Name, T, Size, Init));
  }
  FuncDecl *decl_func(std::string Name, Type *RetTy,
                      std::vector<VarDecl *> Params) {
    return add_decl(new FuncDecl(Name, RetTy, Params));
  }
  StructDecl *decl_struct(Struct *S) { return add_decl(new StructDecl(S)); }
  UnionDecl *decl_union(Union *U) { return add_decl(new UnionDecl(U)); }
  EnumDecl *decl_enum(Enum *E) { return add_decl(new EnumDecl(E)); }
  TypedefDecl *decl_typedef(std::string Name, Type *T) {
    return add_decl(new TypedefDecl(type_typedef(Name, T)));
  }

  // Expr factory APIs (and VarDecl API)
public:
  VarDecl *decl_var(std::string Name, Type *T, Expr *Init = nullptr) {
    return add_decl(new VarDecl(Name, T, Init));
  }
  RawExpr *expr_raw(std::string Val) { return add_expr(new RawExpr(Val)); }
  RawExpr *expr_true() { return expr_raw("true"); }
  RawExpr *expr_false() { return expr_raw("false"); }
  RawExpr *expr_int(int Val) { return expr_raw(std::to_string(Val)); }
  RawExpr *expr_uint(unsigned int Val) {
    return expr_raw(std::to_string(Val) + "u");
  }
  RawExpr *expr_long(long Val) { return expr_raw(std::to_string(Val) + "l"); }
  RawExpr *expr_ulong(unsigned long Val) {
    return expr_raw(std::to_string(Val) + "ul");
  }
  RawExpr *expr_llong(long long Val) {
    return expr_raw(std::to_string(Val) + "ll");
  }
  RawExpr *expr_ullong(unsigned long long Val) {
    return expr_raw(std::to_string(Val) + "ull");
  }
  RawExpr *expr_float(float Val) { return expr_raw(std::to_string(Val) + "f"); }
  RawExpr *expr_double(float Val) { return expr_raw(std::to_string(Val)); }
  RawExpr *expr_char(char Val) {
    return expr_raw(std::string("'") + Val + "'");
  }
  RawExpr *expr_str(std::string Val) { return expr_raw("\"" + Val + "\""); }
  VariableExpr *expr_var(VarDecl *D) { return add_expr(new VariableExpr(D)); }
  RawExpr *expr_var_name(std::string Name) { return expr_raw(Name); }
  SubscriptExpr *expr_subscr(Expr *Base, Expr *Index) {
    return add_expr(new SubscriptExpr(Base, Index));
  }
  CallExpr *expr_call(Expr *Callee, std::vector<Expr *> Args) {
    return add_expr(new CallExpr(Callee, Args));
  }
  // prefix type unary operation
  UnaryOp *expr_pre_unary(std::string Op, Expr *E) {
    return add_expr(new UnaryOp(Op, E, true));
  }
  // postfix type unary operation (post increment/decrement)
  UnaryOp *expr_post_unary(std::string Op, Expr *E) {
    return add_expr(new UnaryOp(Op, E, false));
  }
  BinaryOp *expr_binary(std::string Op, Expr *LHS, Expr *RHS) {
    return add_expr(new BinaryOp(Op, LHS, RHS));
  }
  TernaryOp *expr_ternary(Expr *Cond, Expr *LHS, Expr *RHS) {
    return add_expr(new TernaryOp(Cond, LHS, RHS));
  }
  CastExpr *expr_cast(Type *Ty, Expr *E) {
    return add_expr(new CastExpr(Ty, E));
  }
  ParenExpr *expr_paren(Expr *E) { return add_expr(new ParenExpr(E)); }
  DesignatedInitExpr *expr_designated_init(
      std::vector<std::pair<std::string, Expr *>> Designators) {
    return add_expr(new DesignatedInitExpr(Designators));
  }
  InitListExpr *expr_init_list(std::vector<Expr *> Values) {
    return add_expr(new InitListExpr(Values));
  }

  // Type factory APIs
public:
  RawType *type_raw(std::string Val) { return add_type(new RawType(Val)); }
  RawType *type_char() { return get_or_add_raw_type("char"); }
  RawType *type_uchar() { return get_or_add_raw_type("unsigned char"); }
  RawType *type_short() { return get_or_add_raw_type("short"); }
  RawType *type_ushort() { return get_or_add_raw_type("unsigned short"); }
  RawType *type_int() { return get_or_add_raw_type("int"); }
  RawType *type_uint() { return get_or_add_raw_type("unsigned int"); }
  RawType *type_long() { return get_or_add_raw_type("long"); }
  RawType *type_ulong() { return get_or_add_raw_type("unsigned long"); }
  RawType *type_llong() { return get_or_add_raw_type("long long"); }
  RawType *type_ullong() { return get_or_add_raw_type("unsigned long long"); }
  RawType *type_float() { return get_or_add_raw_type("float"); }
  RawType *type_double() { return get_or_add_raw_type("double"); }
  Void *type_void() { return VoidSingleton.get(); }
  TypeAlias *type_typedef(std::string Name, Type *Ty) {
    return add_type(new TypeAlias(Name, Ty));
  }

  Named *type_name(Decl *D) {
    if (auto P = NamedTypeMap.find(D); P != NamedTypeMap.end()) {
      return P->second;
    }
    auto *Ty = add_type(new Named(D));
    NamedTypeMap[D] = Ty;
    return Ty;
  }
  Pointer *type_ptr(Type *ElmTy) {
    if (auto P = PointerTypeMap.find(ElmTy); P != PointerTypeMap.end()) {
      return P->second;
    }
    auto *Ty = add_type(new Pointer(ElmTy));
    PointerTypeMap[ElmTy] = Ty;
    return Ty;
  }
  Array *type_array(Type *ElmTy, std::vector<Expr *> Size) {
    if (auto P = ArrayTypeMap.find(ElmTy); P != ArrayTypeMap.end()) {
      return P->second;
    }
    auto *Ty = add_type(new Array(ElmTy, Size));
    ArrayTypeMap[ElmTy] = Ty;
    return Ty;
  }
  Function *type_func(Type *RetTy, std::vector<Type *> Params) {
    return add_type(new Function(RetTy, Params));
  }
  Struct *type_struct(std::string Name) { return add_type(new Struct(Name)); }
  Union *type_union(std::string Name) { return add_type(new Union(Name)); }
  Enum *type_enum(std::string Name) { return add_type(new Enum(Name)); }
};
} // namespace namec

#endif // NAMEC_GEN_CONTEXT_H
