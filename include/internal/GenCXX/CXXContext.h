#ifdef NAMEC_GENCXX_CONTEXT_H_CYCLIC
static_assert(false, "Cyclic include detected of " __FILE__);
#endif
#define NAMEC_GENCXX_CONTEXT_H_CYCLIC
#ifndef NAMEC_GENCXX_CONTEXT_H
#define NAMEC_GENCXX_CONTEXT_H

#include "internal/GenCXX/CXXDecl.h"
#include "internal/GenCXX/CXXExprs.h"
#include "internal/GenCXX/CXXForwards.h"
#include "internal/GenCXX/CXXTypes.h"

namespace namecxx {
class UbiquitousDeclStmtMixin;

// Context manage Scope, Decl and Expr heap existence. Stmt are kept in Scope.
// This also exists to eliminate complex circular dependency between Scope,
// Decl, Stmt and Expr.
class Context {
  friend class CFile;
  friend class FuncScope;
  friend class TopLevel;
  friend class UbiquitousDeclStmtMixin;
  friend class ClassTopLevel;
  friend class ClassMemberDeclMixin;

  std::vector<std::unique_ptr<FuncScope>> FuncScopes;
  std::vector<std::unique_ptr<MacroFuncScope>> MacroFuncScopes;
  std::vector<std::unique_ptr<TopLevel>> TopLevels;
  std::vector<std::unique_ptr<ClassTopLevel>> ClassTopLevels;

  std::vector<std::unique_ptr<Decl>> Decls;
  std::vector<std::unique_ptr<Expr>> Exprs;
  std::vector<std::unique_ptr<Type>> Types;
  std::unique_ptr<Void> VoidSingleton;
  std::map<std::string, RawType *> RawTypeMap;
  std::map<Type *, Pointer *> PointerTypeMap;
  std::map<Type *, Reference *> ReferenceTypeMap;
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
  ~Context(){};

  FuncScope *add_func_scope();
  TopLevel *add_top_level();
  MacroFuncScope *add_macro_func_scope();
  ClassTopLevel *add_class_top_level(ClassOrUnion *Cls);

private:
  // Decl factory APIs. Not public to user. Intended to be used by internal
  // VarInitialize in File and Scope.
  RawDecl *decl_raw(std::string Val) { return add_decl(new RawDecl(Val)); }
  ArrayVarDecl *decl_array_var(QualName Name, Type *T, std::vector<Expr *> Size,
                               Expr *Init = nullptr) {
    return add_decl(new ArrayVarDecl(Name, T, Size, Init));
  }
  FuncDecl *decl_func(QualName Name, Type *RetTy, std::vector<VarDecl *> Params,
                      bool IsVarArgs = false) {
    return add_decl(new FuncDecl(*this, Name, RetTy, Params, IsVarArgs));
  }
  FuncSplitDecl *decl_func_split(QualName Name, Type *RetTy,
                                 std::vector<VarDecl *> Params,
                                 bool IsVarArgs = false) {
    return add_decl(new FuncSplitDecl(*this, Name, RetTy, Params, IsVarArgs));
  }
  FuncSplitForwardDecl *decl_func_split_forward(FuncSplitDecl *FD) {
    return add_decl(new FuncSplitForwardDecl(FD));
  }
  FuncTemplateDecl *decl_func_template(QualName Name,
                                       std::vector<VarDecl *> TemplateParams,
                                       Type *RetTy,
                                       std::vector<VarDecl *> Params,
                                       bool IsVarArgs = false) {
    return add_decl(new FuncTemplateDecl(
        *this, decl_func(Name, RetTy, Params, IsVarArgs), TemplateParams));
  }
  EnumDecl *decl_enum(Enum *E, bool IsForward = false) {
    return add_decl(new EnumDecl(E, IsForward));
  }
  TypedefDecl *decl_typedef(QualName Name, Type *T) {
    return add_decl(new TypedefDecl(type_typedef(Name, T)));
  }
  // C++ specific

  RawTemplateDecl *decl_raw_template(QualName Name,
                                     std::vector<VarDecl *> TemplateParams) {
    return add_decl(
        new RawTemplateDecl(*this, decl_raw(Name.to_string()), TemplateParams));
  }

  ArrayVarTemplateDecl *
  decl_array_var_template(QualName Name, std::vector<VarDecl *> TemplateParams,
                          Type *T, std::vector<Expr *> Size,
                          Expr *Init = nullptr) {
    return add_decl(new ArrayVarTemplateDecl(
        *this, decl_array_var(Name, T, Size, Init), TemplateParams));
  }
  VarInitializeDecl *decl_var_init(QualName Name, Type *T,
                                   std::vector<Expr *> Args,
                                   bool IsListInit = false) {
    return add_decl(new VarInitializeDecl(Name, T, Args, IsListInit));
  }
  VarInitializeTemplateDecl *
  decl_var_init_template(QualName Name, std::vector<VarDecl *> TemplateParams,
                         Type *T, std::vector<Expr *> Args,
                         bool IsListInit = false) {
    return add_decl(new VarInitializeTemplateDecl(
        *this, decl_var_init(Name, T, Args, IsListInit), TemplateParams));
  }
  ClassDecl *decl_class(Class *T, bool IsForward = false) {
    return add_decl(new ClassDecl(T, IsForward));
  }
  ClassTemplateDecl *
  decl_class_template(Class *T, std::vector<VarDecl *> TemplateParams) {
    return add_decl(
        new ClassTemplateDecl(*this, decl_class(T, false), TemplateParams));
  }
  UnionDecl *decl_union(Union *T, bool IsForward = false) {
    return add_decl(new UnionDecl(T, IsForward));
  }
  UnionTemplateDecl *
  decl_union_template(Union *T, std::vector<VarDecl *> TemplateParams) {
    return add_decl(
        new UnionTemplateDecl(*this, decl_union(T, false), TemplateParams));
  }
  MethodDecl *decl_method(QualName Name, Type *RetTy,
                          std::vector<VarDecl *> Params,
                          bool IsVarArgs = false) {
    return add_decl(new MethodDecl(*this, Name, RetTy, Params, IsVarArgs));
  }
  MethodSplitDecl *decl_method_split(QualName Name, ClassOrUnion *C,
                                     Type *RetTy, std::vector<VarDecl *> Params,
                                     bool IsVarArgs = false) {
    return add_decl(
        new MethodSplitDecl(*this, C, Name, RetTy, Params, IsVarArgs));
  }
  MethodSplitForwardDecl *decl_method_split_forward(MethodSplitDecl *MD) {
    return add_decl(new MethodSplitForwardDecl(MD));
  }

  CtorDecl *decl_ctor(QualName Name, std::vector<VarDecl *> Params,
                      bool IsVarArgs = false) {
    return add_decl(new CtorDecl(*this, Name, Params, IsVarArgs));
  }
  CtorSplitDecl *decl_ctor_split(ClassOrUnion *C, std::vector<VarDecl *> Params,
                                 bool IsVarArgs = false) {
    return add_decl(new CtorSplitDecl(*this, C, Params, IsVarArgs));
  }
  CtorSplitForwardDecl *decl_ctor_split_forward(CtorSplitDecl *CD) {
    return add_decl(new CtorSplitForwardDecl(CD));
  }
  MethodTemplateDecl *
  decl_method_template(QualName Name, std::vector<VarDecl *> TemplateParams,
                       Type *RetTy, std::vector<VarDecl *> Params,
                       bool IsVarArgs = false) {
    return add_decl(new MethodTemplateDecl(
        *this, decl_method(Name, RetTy, Params, IsVarArgs), TemplateParams));
  }
  EnumDecl *decl_enum_class(EnumClass *T, bool IsForward = false) {
    return add_decl(new EnumDecl(T, IsForward));
  }
  UsingDecl *decl_using(QualName Name, Type *T) {
    return add_decl(new UsingDecl(type_typedef(Name, T)));
  }
  UsingTemplateDecl *decl_using_template(QualName Name,
                                         std::vector<VarDecl *> TemplateParams,
                                         Type *T) {
    return add_decl(
        new UsingTemplateDecl(*this, decl_using(Name, T), TemplateParams));
  }
  // QualName handling implementations.
  std::vector<std::string> qual_name_concat(QualName Top,
                                            std::vector<std::string> Other);
  std::vector<std::string> qual_name_concat(Decl *Top,
                                            std::vector<std::string> Other);
  template <typename T> std::vector<std::string> qn_impl(T Name) {
    return qual_name_concat(Name, {});
  }
  template <typename T, typename... Ts>
  std::vector<std::string> qn_impl(T Name, Ts... Names) {
    return qual_name_concat(Name, qn_impl(Names...));
  }

  // Expr factory APIs (and VarDecl API)
public:
  VarDecl *decl_var(QualName Name, Type *T, Expr *Init = nullptr) {
    return add_decl(new VarDecl(Name, T, Init));
  }
  VarTemplateDecl *decl_var_template(QualName Name,
                                     std::vector<VarDecl *> TemplateParams,
                                     Type *T, Expr *Init = nullptr) {
    return add_decl(
        new VarTemplateDecl(*this, decl_var(Name, T, Init), TemplateParams));
  }

  QualName qual_name(std::vector<std::string> Name) { return QualName(Name); }
  // Super shortcut API for qual_name used as construction and concatenation.
  // Accept string, QualName and Decl.
  template <typename... T> QualName QN(T... Names) {
    return qual_name(qn_impl(Names...));
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
  RawExpr *expr_var_name(QualName Name) { return expr_raw(Name.to_string()); }
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
  InitListExpr *expr_init_list(std::vector<Expr *> Values) {
    return add_expr(new InitListExpr(Values));
  }
  CallExpr *expr_va_arg(Expr *VaList, Type *T) {
    return expr_call(expr_var_name("va_arg"), {VaList, expr_type(T)});
  }
  CallExpr *expr_sizeof(Type *T) {
    return expr_call(expr_var_name("sizeof"), {expr_type(T)});
  }
  // C++ specific.
  RawExpr *expr_type(Type *T) { return expr_raw(T->to_string()); }
  QualNameExpr *expr_qual_name(std::vector<std::string> Name) {
    return add_expr(new QualNameExpr(qual_name(Name)));
  }
  LambdaExpr *expr_lambda(std::vector<Expr *> Captures,
                          std::vector<VarDecl *> Params, bool IsVarArgs = false,
                          Type *RetTy = nullptr) {
    return add_expr(new LambdaExpr(*this, Captures, Params, IsVarArgs, RetTy));
  }
  NewExpr *expr_new(Type *T, std::vector<Expr *> Args, bool IsListInit = false,
                    Expr *ArraySize = nullptr, Expr *Placement = nullptr) {
    auto *New = new NewExpr(T, Args, IsListInit);
    New->set_array_size(ArraySize);
    New->set_placement(Placement);
    return add_expr(New);
  }
  DeleteExpr *expr_delete(Expr *Val, bool IsArray = false) {
    return add_expr(new DeleteExpr(Val, IsArray));
  }
  UserDefinedLiteral *expr_user_defined_literal(Expr *E, std::string Postfix) {
    return add_expr(new UserDefinedLiteral(E, Postfix));
  }
  QualNameExpr *expr_qual_name(QualName Name) {
    return add_expr(new QualNameExpr(Name));
  }
  InstantiateExpr *expr_instantiate(TemplateDecl *TD,
                                    std::vector<TypeOrExpr *> Args) {
    return add_expr(new InstantiateExpr(TD, Args));
  }
  InstantiateExpr *expr_raw_instantiate(QualName Name,
                                        std::vector<TypeOrExpr *> Args) {
    return expr_instantiate(decl_raw_template(Name.to_string(), {}), Args);
  }
  PackExpansionExpr *expr_pack_expansion(Expr *E) {
    return add_expr(new PackExpansionExpr(E));
  }
  FoldExpr *expr_fold(std::string Op, Expr *Pack, Expr *Init, bool IsLeftFold) {
    return add_expr(new FoldExpr(Op, Pack, Init, IsLeftFold));
  }

  // handy expr APIs
  UnaryOp *expr_addr(Expr *E) { return expr_pre_unary("&", E); }
  UnaryOp *expr_deref(Expr *E) { return expr_pre_unary("*", E); }
  UnaryOp *expr_not(Expr *E) { return expr_pre_unary("!", E); }
  BinaryOp *expr_dot(Expr *LHS, QualName RHS) {
    return expr_binary(".", LHS, expr_qual_name(RHS));
  }
  BinaryOp *expr_arrow(Expr *LHS, QualName RHS) {
    return expr_binary("->", LHS, expr_qual_name(RHS));
  }
  BinaryOp *expr_eq(Expr *LHS, Expr *RHS) {
    return expr_binary("==", LHS, RHS);
  }
  BinaryOp *expr_lt(Expr *LHS, Expr *RHS) { return expr_binary("<", LHS, RHS); }
  BinaryOp *expr_gt(Expr *LHS, Expr *RHS) { return expr_binary(">", LHS, RHS); }
  BinaryOp *expr_leq(Expr *LHS, Expr *RHS) {
    return expr_binary("<=", LHS, RHS);
  }
  BinaryOp *expr_geq(Expr *LHS, Expr *RHS) {
    return expr_binary(">=", LHS, RHS);
  }

  // Type factory APIs
public:
  RawType *type_raw(std::string Val) { return add_type(new RawType(Val)); }
  Named *type_var(VarDecl *D) { return type_name(D); }
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
  Void *type_void() { return VoidSingleton.get(); }
  TypeAlias *type_typedef(QualName Name, Type *Ty) {
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
  Reference *type_ref(Type *ElmTy) {
    if (auto P = ReferenceTypeMap.find(ElmTy); P != ReferenceTypeMap.end()) {
      return P->second;
    }
    auto *Ty = add_type(new Reference(ElmTy));
    ReferenceTypeMap[ElmTy] = Ty;
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
  Type *type_va_list() { return get_or_add_raw_type("va_list"); }

  // C++ specific.
  Class *type_class(QualName Name,
                    std::vector<std::pair<AccessSpec, Type *>> Bases = {},
                    bool IsStruct = false) {
    return add_type(new Class(*this, Name, Bases, IsStruct));
  }
  Union *type_union(QualName Name) { return add_type(new Union(*this, Name)); }
  Enum *type_enum(QualName Name, Type *BaseType = nullptr) {
    return add_type(new Enum(Name, BaseType));
  }
  Instantiation *type_instantiate(TemplateDecl *TD,
                                  std::vector<TypeOrExpr *> Args) {
    return add_type(new Instantiation(TD, Args));
  }
  Instantiation *type_raw_instantiate(QualName Name,
                                      std::vector<TypeOrExpr *> Args) {
    return type_instantiate(decl_raw_template(Name.to_string(), {}), Args);
  }
  PackedType *type_packed(Type *T) { return add_type(new PackedType(T)); }
  TypeNameType *type_type_name(bool IsClass = false) {
    return add_type(new TypeNameType(IsClass));
  }
};
} // namespace namecxx

#endif // NAMEC_GENCXX_CONTEXT_H
#undef NAMEC_GENCXX_CONTEXT_H_CYCLIC
