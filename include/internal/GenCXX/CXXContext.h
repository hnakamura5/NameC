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
class Context
    : public LiteralExprAPIMixin<Context, Expr, RawExpr>,
      public CommonBasicExprAPIMixin<Context, Expr, UnaryOp, BinaryOp,
                                     TernaryOp, SubscriptExpr, CallExpr,
                                     CastExpr, ParenExpr, Type>,
      public ShortFormExprAPIMixin<Context, Expr, UnaryOp, BinaryOp, CallExpr,
                                   Type, VarDecl, QualName>,
      public BuiltinTypeAPIMixin<Context, Type, RawType, Void> {
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

protected:
  void on_add_raw_expr(Expr *E) override { add_expr(E); }
  void on_add_expr(Expr *E) override { add_expr(E); }
  void on_add_type(Type *T) override { add_type(T); }

public:
  Context()
      : LiteralExprAPIMixin(*this), CommonBasicExprAPIMixin(*this),
        ShortFormExprAPIMixin(*this), BuiltinTypeAPIMixin(*this) {}
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

  RawExpr *expr_true() { return expr_raw("true"); }
  RawExpr *expr_false() { return expr_raw("false"); }

  RawExpr *expr_nullptr() { return expr_raw("nullptr"); }
  VariableExpr *expr_var(VarDecl *D) { return add_expr(new VariableExpr(D)); }
  RawExpr *expr_var_name(QualName Name) { return expr_raw(Name.to_string()); }

  InitListExpr *expr_init_list(std::vector<Expr *> Values) {
    return add_expr(new InitListExpr(Values));
  }
  Expr *expr_qual_name(QualName Name) {
    return add_expr(new QualNameExpr(Name));
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

  // Short form factory methods
  template <typename... T> QualName QN(T... Names) {
    return qual_name(qn_impl(Names...));
  }
  Expr *EX(QualName Name) { return expr_qual_name(Name); }
  Expr *EX(TemplateDecl *TD, std::vector<TypeOrExpr *> Args) {
    return expr_instantiate(TD, Args);
  }

  // Type factory APIs
public:
  TypeAlias *type_typedef(QualName Name, Type *Ty) {
    return add_type(new TypeAlias(Name, Ty));
  }

  Named *type_var(VarDecl *D) { return type_name(D); }
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
