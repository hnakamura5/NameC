#ifndef UTIL_COMMONMIXINS_H
#define UTIL_COMMONMIXINS_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace namec_util {
/**
  @brief Supports literal like expr factory API mixin used by both
  namec::Context and namecxx::Context
 */
template <typename Context, typename Expr, typename RawExpr>
class LiteralExprAPIMixin {
  Context &C;

protected:
  virtual void on_add_raw_expr(Expr *E) = 0;

public:
  LiteralExprAPIMixin(Context &C) : C(C) {}
  // Literal API
  RawExpr *expr_raw(std::string Val) {
    auto *Raw = new RawExpr(Val);
    on_add_raw_expr(Raw);
    return Raw;
  }
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
};

/**
  @brief Supports basic expr factory API mixin used by both namec::Context and
  namecxx::Context
 */
template <typename Context, typename Expr, typename UnaryOp, typename BinaryOp,
          typename TernaryOp, typename SubscriptExpr, typename CallExpr,
          typename CastExpr, typename ParenExpr, typename Type>
class CommonBasicExprAPIMixin {
  // We want first class module (first class namespace?) here...
  Context &C;
  template <typename T> T *add_expr(T *E) {
    on_add_expr(E);
    return E;
  }

protected:
  virtual void on_add_expr(Expr *E) = 0;

public:
  CommonBasicExprAPIMixin(Context &C) : C(C) {}
  // Common expr APIs
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
  SubscriptExpr *expr_subscr(Expr *Base, Expr *Index) {
    return add_expr(new SubscriptExpr(Base, Index));
  }
  SubscriptExpr *expr_subscr(Expr *Base, std::vector<Expr *> Indexes) {
    if (Indexes.empty()) {
      // Returning Base[]
      return expr_subscr(Base, C.expr_raw(""));
    }
    auto *Current = Base;
    SubscriptExpr *Result = nullptr;
    for (auto *Index : Indexes) {
      Current = (Result = expr_subscr(Current, Index));
    }
    return Result;
  }
  CallExpr *expr_call(Expr *Callee, std::vector<Expr *> Args) {
    return add_expr(new CallExpr(Callee, Args));
  }

  CastExpr *expr_cast(Type *Ty, Expr *E) {
    return add_expr(new CastExpr(Ty, E));
  }
  ParenExpr *expr_paren(Expr *E) { return add_expr(new ParenExpr(E)); }
};

/**
  @brief Supports short form expr factory API mixin used by both namec::Context
  and namecxx::Context
 */
template <typename Context, typename Expr, typename UnaryOp, typename BinaryOp,
          typename CallExpr, typename Type, typename VarDecl, typename NameT>
class ShortFormExprAPIMixin {
  Context &C;

public:
  ShortFormExprAPIMixin(Context &C) : C(C) {}

  // handy expr APIs
  UnaryOp *expr_addr(Expr *E) { return C.expr_pre_unary("&", E); }
  UnaryOp *expr_deref(Expr *E) { return C.expr_pre_unary("*", E); }
  UnaryOp *expr_not(Expr *E) { return C.expr_pre_unary("!", E); }
  BinaryOp *expr_dot(Expr *LHS, NameT RHS) {
    return C.expr_binary(".", LHS, expr_var_name(RHS));
  }
  BinaryOp *expr_arrow(Expr *LHS, NameT RHS) {
    return C.expr_binary("->", LHS, expr_var_name(RHS));
  }
  BinaryOp *expr_eq(Expr *LHS, Expr *RHS) {
    return C.expr_binary("==", LHS, RHS);
  }
  BinaryOp *expr_lt(Expr *LHS, Expr *RHS) {
    return C.expr_binary("<", LHS, RHS);
  }
  BinaryOp *expr_gt(Expr *LHS, Expr *RHS) {
    return C.expr_binary(">", LHS, RHS);
  }
  BinaryOp *expr_leq(Expr *LHS, Expr *RHS) {
    return C.expr_binary("<=", LHS, RHS);
  }
  BinaryOp *expr_geq(Expr *LHS, Expr *RHS) {
    return C.expr_binary(">=", LHS, RHS);
  }
  CallExpr *expr_va_arg(Expr *VaList, Type *T) {
    return C.expr_call(C.expr_var_name("va_arg"), {VaList, C.expr_type(T)});
  }
  CallExpr *expr_sizeof(Type *T) {
    return C.expr_call(C.expr_var_name("sizeof"), {C.expr_type(T)});
  }

  // Short form factory methods
  Expr *EX(std::nullptr_t) { return C.expr_nullptr(); }
  Expr *EX(Expr *E) { return C.expr_paren(E); }
  Expr *EX(VarDecl *VD) { return C.expr_var(VD); }
  Expr *EX(bool Val) { return Val ? C.expr_true() : C.expr_false(); }
  Expr *EX(signed char Val) { return C.expr_char(Val); }
  Expr *EX(int Val) { return C.expr_int(Val); }
  Expr *EX(unsigned int Val) { return C.expr_uint(Val); }
  Expr *EX(long Val) { return C.expr_long(Val); }
  Expr *EX(unsigned long Val) { return C.expr_ulong(Val); }
  Expr *EX(long long Val) { return C.expr_llong(Val); }
  Expr *EX(unsigned long long Val) { return C.expr_ullong(Val); }
  Expr *EX(float Val) { return C.expr_float(Val); }
  Expr *EX(double Val) { return C.expr_double(Val); }
  Expr *EX(char Val) { return C.expr_char(Val); }
  Expr *EX(const char *Val) { return C.expr_raw(Val); }
  Expr *EX(std::string Val) { return C.expr_raw(Val); }
  template <typename T> Expr *EX(std::string Op, T E) {
    return C.expr_paren(C.expr_pre_unary(Op, EX(E)));
  }
  template <typename T1, typename T2> Expr *EX(T1 LHS, std::string Op, T2 RHS) {
    return C.expr_paren(C.expr_binary(Op, EX(LHS), EX(RHS)));
  }
  VarDecl *VAR(std::string Name, Type *T, Expr *Init = nullptr) {
    return decl_var(Name, T, Init);
  }
  Expr *STR(std::string Val) { return C.expr_str(Val); }
  template <typename CalleeT, typename... T>
  Expr *CALL(CalleeT Callee, T... Args) {
    return C.expr_paren(C.expr_call(EX(Callee), {EX(Args)...}));
  }
  template <typename BaseT, typename... T> Expr *IDX(BaseT Base, T... Indexes) {
    return C.expr_paren(C.expr_subscr(EX(Base), {EX(Indexes)...}));
  }
};

/**
  @brief Supports builtin type factory API mixin used by both namec::Context and
  namecxx::Context
 */
template <typename Context, typename Type, typename RawType, typename Void>
class BuiltinTypeAPIMixin {
  Context &C;

  std::unique_ptr<Void> VoidSingleton;
  std::map<std::string, RawType *> RawTypeMap;

protected:
  virtual void on_add_type(Type *T) = 0;

  RawType *get_or_add_raw_type(std::string Val) {
    if (auto P = RawTypeMap.find(Val); P != RawTypeMap.end()) {
      return P->second;
    }
    auto *Raw = new RawType(Val);
    on_add_type(Raw);
    RawTypeMap[Val] = Raw;
    return Raw;
  }

public:
  BuiltinTypeAPIMixin(Context &C) : C(C) { VoidSingleton.reset(new Void()); }
  RawType *type_raw(std::string Val) { return get_or_add_raw_type(Val); }
  RawType *type_auto() { return get_or_add_raw_type("auto"); }
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
  RawType *type_va_list() { return get_or_add_raw_type("va_list"); }
  Void *type_void() { return VoidSingleton.get(); }
};

} // namespace namec_util

#endif // UTIL_COMMONMIXINS_H
