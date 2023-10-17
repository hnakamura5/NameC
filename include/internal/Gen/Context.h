#ifndef NAMEC_GEN_CONTEXT_H
#define NAMEC_GEN_CONTEXT_H

#include "Util/CommonMixins.h"
#include "internal/Gen/Decl.h"
#include "internal/Gen/Exprs.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/Types.h"

namespace namec {
class UbiquitousDeclStmtMixin;

/**
  @brief Context is global heap manager, and also a factory for types and
  expressions.

  This create and holds ownership of types and expressions used independently
  from the specific location in TopLevel or Scopes. Stmt are kept in Scopes.

  In the viewpoint of Implementation, this also exists to eliminate complex
  circular dependency between Scope, Decl, Stmt and Expr.

 ## Short form expression factory methods

  There are short form factory for frequently used expressions. These methods
are inherited from ShortFormExprAPIMixin.

  The difference from normal factory methods is, Short form ensures operator
  association by inserting parentheses.

  @li VAR() : Short form for variable declaration.
  @li EX() : Short form for raw literals, variable names, and unary/binary
operations.
  @li STR() : string literal.
  @li CALL() : function call.
  @li IDX() : array subscript.
  @li and some other expr_* methods.

  @note Attention to dispatching and grotesque errors from template!

  ## Examples

  Let C be a Context object.

  ### Binary Operation
  ```cpp
  int i1 = 1;
  int i2 = 2;
  Expr *E = C.expr_binary("+", C.expr_int(i1), C.expr_int(i2));
  E = C.EX(i1, "+", i2); // short form
  ```

  then E can be used as the expression passed to statements of,

  ```c
  1 + 2
  ```

  ### Cast Expression

  ```cpp
  Expr *E = C.expr_cast(C.type_ptr(C.type_int()), C.expr_addr(C.expr_raw("x")));
  // NO shorter form for cast.
  ```

  then E is,

  ```c
  (int *) &x
  ```
 */
class Context
    : public LiteralExprAPIMixin<Context, Expr, RawExpr>,
      public CommonBasicExprAPIMixin<Context, Expr, UnaryOp, BinaryOp,
                                     TernaryOp, SubscriptExpr, CallExpr,
                                     CastExpr, ParenExpr, Type>,
      public ShortFormExprAPIMixin<Context, Expr, UnaryOp, BinaryOp, CallExpr,
                                   Type, VarDecl, std::string>,
      public BuiltinTypeAPIMixin<Context, Type, RawType, Void> {
  friend class CFile;
  friend class FuncScope;
  friend class TopLevel;
  friend class UbiquitousDeclStmtMixin;

  std::vector<std::unique_ptr<FuncScope>> Scopes;
  std::vector<std::unique_ptr<Decl>> Decls;
  std::vector<std::unique_ptr<Expr>> Exprs;
  std::vector<std::unique_ptr<Type>> Types;
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

protected:
  void on_add_raw_expr(Expr *E) override { add_expr(E); }
  void on_add_expr(Expr *E) override { add_expr(E); }
  void on_add_type(Type *E) override { add_type(E); }

public:
  Context()
      : LiteralExprAPIMixin(*this), CommonBasicExprAPIMixin(*this),
        ShortFormExprAPIMixin(*this), BuiltinTypeAPIMixin(*this) {}
  ~Context() = default;

  FuncScope *add_scope();

private:
  // Decl factory APIs. Not public to user. Intended to be used by internal
  // VarInitialize in File and Scope.
  RawDecl *decl_raw(std::string Val) { return add_decl(new RawDecl(Val)); }

  FuncDecl *decl_func(std::string Name, Type *RetTy,
                      std::vector<VarDecl *> Params, bool IsVarArgs = false) {
    return add_decl(new FuncDecl(*this, Name, RetTy, Params, IsVarArgs));
  }
  FuncSplitDecl *decl_func_split(std::string Name, Type *RetTy,
                                 std::vector<VarDecl *> Params,
                                 bool IsVarArgs = false) {
    return add_decl(new FuncSplitDecl(*this, Name, RetTy, Params, IsVarArgs));
  }
  FuncSplitForwardDecl *decl_func_split_forward(FuncSplitDecl *FD) {
    return add_decl(new FuncSplitForwardDecl(FD));
  }
  StructDecl *decl_struct(Struct *S, bool IsForward = false) {
    return add_decl(new StructDecl(S, IsForward));
  }
  UnionDecl *decl_union(Union *U, bool IsForward = false) {
    return add_decl(new UnionDecl(U, IsForward));
  }
  EnumDecl *decl_enum(Enum *E, bool IsForward = false) {
    return add_decl(new EnumDecl(E, IsForward));
  }
  TypedefDecl *decl_typedef(std::string Name, Type *T) {
    return add_decl(new TypedefDecl(type_typedef(Name, T)));
  }

  // Expr factory APIs (and VarDecl API)
public:
  /// @brief This is used
  VarDecl *decl_var(std::string Name, Type *T, Expr *Init = nullptr) {
    return add_decl(new VarDecl(Name, T, Init));
  }
  ArrayVarDecl *decl_array_var(std::string Name, Type *T,
                               std::vector<Expr *> Size, Expr *Init = nullptr) {
    return add_decl(new ArrayVarDecl(Name, T, Size, Init));
  }
  RawExpr *expr_true() { return expr_raw("1"); }
  RawExpr *expr_false() { return expr_raw("0"); }

  RawExpr *expr_nullptr() { return expr_raw("((void*)0)"); }
  VariableExpr *expr_var(VarDecl *D) { return add_expr(new VariableExpr(D)); }
  RawExpr *expr_var_name(std::string Name) { return expr_raw(Name); }

  DesignatedInitExpr *expr_designated_init(
      std::vector<std::pair<std::string, Expr *>> Designators) {
    return add_expr(new DesignatedInitExpr(Designators));
  }
  InitListExpr *expr_init_list(std::vector<Expr *> Values) {
    return add_expr(new InitListExpr(Values));
  }
  GenericSelection *expr_generic_selection(Expr *ControllingExpr) {
    return add_expr(new GenericSelection(ControllingExpr));
  }
  RawExpr *expr_type(Type *T) { return expr_raw(T->to_string()); }

  // Type factory APIs
public:
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
  Function *type_func(Type *RetTy, std::vector<Type *> Params,
                      bool IsVarArgs = false) {
    return add_type(new Function(RetTy, Params, IsVarArgs));
  }
  Struct *type_struct(std::string Name) { return add_type(new Struct(Name)); }
  Union *type_union(std::string Name) { return add_type(new Union(Name)); }
  Enum *type_enum(std::string Name) { return add_type(new Enum(Name)); }
};
} // namespace namec

#endif // NAMEC_GEN_CONTEXT_H
