#ifdef NAMEC_GENCXX_DECL_H_CYCLIC
#error "Cyclic include of CXXDecl.h detected."
#endif
#define NAMEC_GENCXX_DECL_H_CYCLIC
#ifndef NAMEC_GENCXX_DECL_H
#define NAMEC_GENCXX_DECL_H

#include "internal/GenCXX/CXXCommon.h"
#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {
class Decl : public Emit {
  std::vector<Attribute *> Attrs;

public:
  virtual ~Decl() = default;
  virtual void emit_impl(std::ostream &SS) = 0;
  virtual QualName get_name() = 0;
  std::string get_name_str() { return get_name().to_string(); }
  void add_attr(Attribute *Attr) { Attrs.push_back(Attr); }
};

class RawDecl : public Decl {
  std::string Val;

public:
  RawDecl(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }
  QualName get_name() override { return Val; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class VarDecl : public Decl {
  Type *Ty;
  QualName Name;
  Expr *Init = nullptr;

  bool IsConst = false;
  bool IsExtern = false;
  bool IsStatic = false;
  bool IsVolatile = false;
  bool IsRestrict = false;
  bool IsInline = false;

public:
  VarDecl(QualName Name, Type *Ty) : Ty(Ty), Name(Name) {}
  VarDecl(QualName Name, Type *Ty, Expr *Init)
      : Ty(Ty), Name(Name), Init(Init) {}
  Type *get_type() { return Ty; }
  Expr *get_init() { return Init; }
  void set_const(bool IsConst) { this->IsConst = IsConst; }
  bool is_const() { return IsConst; }
  void set_extern(bool IsExtern) { this->IsExtern = IsExtern; }
  bool is_extern() { return IsExtern; }
  void set_static(bool IsStatic) { this->IsStatic = IsStatic; }
  bool is_static() { return IsStatic; }
  void set_volatile(bool IsVolatile) { this->IsVolatile = IsVolatile; }
  bool is_volatile() { return IsVolatile; }
  void set_restrict(bool IsRestrict) { this->IsRestrict = IsRestrict; }
  bool is_restrict() { return IsRestrict; }
  bool is_inline() { return IsInline; }
  QualName get_name() override { return Name; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ArrayVarDecl : public VarDecl {
  std::vector<Expr *> Size;

public:
  using size_iterator = decltype(Size)::iterator;
  ArrayVarDecl(QualName Name, Type *Ty, std::vector<Expr *> Size)
      : VarDecl(Name, Ty), Size(Size) {}
  ArrayVarDecl(QualName Name, Type *Ty, std::vector<Expr *> Size, Expr *Init)
      : VarDecl(Name, Ty, Init), Size(Size) {}
  std::vector<Expr *> get_size() { return Size; }
  IteratorRange<size_iterator> sizes() {
    return IteratorRange<size_iterator>(Size.begin(), Size.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class VarInitializeDecl : public VarDecl {
  std::vector<Expr *> Args;
  bool IsListInit;

public:
  using arg_iterator = decltype(Args)::iterator;
  VarInitializeDecl(QualName Name, Type *Ty, std::vector<Expr *> Args,
                    bool IsListInit)
      : VarDecl(Name, Ty), Args(Args), IsListInit(IsListInit) {}
  IteratorRange<arg_iterator> args() {
    return IteratorRange<arg_iterator>(Args.begin(), Args.end());
  }
  bool is_list_init() { return IsListInit; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TypedefDecl : public Decl {
  TypeAlias *TA;

public:
  TypedefDecl(TypeAlias *TA) : TA(TA) {}
  TypeAlias *get_type_alias() { return TA; }
  QualName get_name() override;

protected:
  void emit_impl(std::ostream &SS) override;
};

// using Name = T;
class UsingDecl : public Decl {
  TypeAlias *TA;

public:
  UsingDecl(TypeAlias *TA) : TA(TA) {}
  TypeAlias *get_type_alias() { return TA; }
  QualName get_name() override;

protected:
  void emit_impl(std::ostream &SS) override;
};

class FuncSplitForwardDecl;
class FuncDecl : public Decl {
  friend class FuncSplitForwardDecl;

protected:
  Context &C;
  Type *RetTy;
  QualName Name;
  // Default argument is by init of VarDecl.
  std::vector<VarDecl *> Params;
  // Empty for declaration
  FuncScope *Body = nullptr;
  bool IsVarArg;

  bool IsExtern = false;
  bool IsStatic = false;
  bool IsConstExpr = false;
  bool IsInline = false;

public:
  using param_iterator = decltype(Params)::iterator;
  FuncDecl(Context &C, QualName Name, Type *RetTy,
           std::vector<VarDecl *> Params, bool IsVarArg)
      : C(C), RetTy(RetTy), Name(Name), Params(Params), IsVarArg(IsVarArg) {}
  Type *get_ret_type() { return RetTy; }
  IteratorRange<param_iterator> params() {
    return IteratorRange<param_iterator>(Params.begin(), Params.end());
  }
  std::vector<VarDecl *> get_params() { return Params; }
  FuncScope *get_or_add_body();
  void set_extern(bool IsExtern) { this->IsExtern = IsExtern; }
  bool is_extern() { return IsExtern; }
  void set_static(bool IsStatic) { this->IsStatic = IsStatic; }
  bool is_static() { return IsStatic; }
  void set_constexpr(bool IsConstExpr) { this->IsConstExpr = IsConstExpr; }
  bool is_constexpr() { return IsConstExpr; }
  virtual bool is_forward() { return !Body; }
  void set_inline(bool IsInline) { this->IsInline = IsInline; }
  bool is_inline() { return IsInline; }
  bool is_vararg() { return IsVarArg; }
  QualName get_name() override { return Name; }
  virtual bool is_split_definition() { return false; }

protected:
  virtual void emit_impl_impl(std::ostream &SS, bool IsForward,
                              bool IsSplitDefinition);
  void emit_impl(std::ostream &SS) override;
};

class FuncSplitDecl : public FuncDecl {

public:
  using FuncDecl::FuncDecl;
  bool is_split_definition() override { return true; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class FuncSplitForwardDecl : public Decl {

protected:
  FuncSplitDecl *FD;

public:
  FuncSplitForwardDecl(FuncSplitDecl *FD) : FD(FD) {}
  FuncDecl *get_func_decl() { return FD; }
  QualName get_name() override { return FD->get_name(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

class ClassDecl : public Decl {
  Class *C;
  bool IsForward;

public:
  ClassDecl(Class *C, bool IsForward) : C(C), IsForward(IsForward) {}
  Class *get_class() { return C; }
  bool is_forward() { return IsForward; }
  QualName get_name() override;

protected:
  void emit_impl(std::ostream &SS) override;
};

class UnionDecl : public Decl {
  Union *U;
  bool IsForward = false;

public:
  UnionDecl(Union *U, bool IsForward) : U(U), IsForward(IsForward) {}
  Union *get_union() { return U; }
  bool is_forward() { return IsForward; }
  QualName get_name() override;

protected:
  void emit_impl(std::ostream &SS) override;
};

class MethodSplitForwardDecl;
class MethodDecl : public FuncDecl {
  friend class MethodSplitForwardDecl;

protected:
  bool IsOverride = false;
  bool IsVirtual = false;
  bool IsDelete = false;
  bool IsDefault = false;
  bool IsAbstract = false;
  bool IsConst = false;
  bool IsFinal = false;

public:
  MethodDecl(Context &C, QualName Name, Type *RetTy,
             std::vector<VarDecl *> Params, bool IsVarArg)
      : FuncDecl(C, Name, RetTy, Params, IsVarArg) {}
  void set_override(bool IsOverride) { this->IsOverride = IsOverride; }
  bool is_override() { return IsOverride; }
  void set_virtual(bool IsVirtual) { this->IsVirtual = IsVirtual; }
  bool is_virtual() { return IsVirtual; }
  void set_delete(bool IsDelete) { this->IsDelete = IsDelete; }
  bool is_delete() { return IsDelete; }
  void set_default(bool IsDefault) { this->IsDefault = IsDefault; }
  bool is_default() { return IsDefault; }
  void set_abstract(bool IsAbstract) { this->IsAbstract = IsAbstract; }
  bool is_abstract() { return IsAbstract; }
  void set_const(bool IsConst) { this->IsConst = IsConst; }
  bool is_const() { return IsConst; }
  void set_final(bool IsFinal) { this->IsFinal = IsFinal; }
  bool is_final() { return IsFinal; }
  bool is_forward() override {
    return !Body && !IsAbstract && !IsDefault && !IsDelete;
  }

protected:
  void emit_impl_impl(std::ostream &SS, bool IsForward,
                      bool IsSplitDefinition) override;
  void emit_impl(std::ostream &SS) override;
};

class MethodSplitDecl : public MethodDecl {
  ClassOrUnion *Parent;

public:
  MethodSplitDecl(Context &C, ClassOrUnion *Parent, QualName Name, Type *RetTy,
                  std::vector<VarDecl *> Params, bool IsVarArg);
  ClassOrUnion *get_parent() { return Parent; }

protected:
  bool is_split_definition() override { return true; }
  void emit_impl(std::ostream &SS) override;
};

class MethodSplitForwardDecl : public Decl {
  MethodSplitDecl *MD;

public:
  MethodSplitForwardDecl(MethodSplitDecl *MD) : MD(MD) {}
  MethodDecl *get_method_decl() { return MD; }
  QualName get_name() override { return MD->get_name(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

class CtorSplitForwardDecl;
class CtorDecl : public MethodDecl {
  friend class CtorSplitForwardDecl;
  std::vector<std::pair<QualName, Expr *>> Inits;

public:
  using init_iterator = decltype(Inits)::iterator;
  CtorDecl(Context &C, QualName Name, std::vector<VarDecl *> Params,
           bool IsVarArg);
  void add_init(QualName Name, Expr *Init) { Inits.push_back({Name, Init}); }
  IteratorRange<init_iterator> inits() {
    return IteratorRange<init_iterator>(Inits.begin(), Inits.end());
  }

protected:
  void emit_impl_impl(std::ostream &SS, bool IsForward,
                      bool IsSplitDefinition) override;
  void emit_impl(std::ostream &SS) override;
};

class CtorSplitDecl : public CtorDecl {
  ClassOrUnion *Parent;

public:
  CtorSplitDecl(Context &C, ClassOrUnion *Parent, std::vector<VarDecl *> Params,
                bool IsVarArg);
  ClassOrUnion *get_parent() { return Parent; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class CtorSplitForwardDecl : public Decl {
  CtorSplitDecl *CD;

public:
  CtorSplitForwardDecl(CtorSplitDecl *CD) : CD(CD) {}
  CtorDecl *get_ctor_decl() { return CD; }
  QualName get_name() override { return CD->get_name(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

class EnumDecl : public Decl {
  Enum *E;
  bool IsForward = false;

public:
  EnumDecl(Enum *E, bool IsForward) : E(E), IsForward(IsForward) {}
  Enum *get_enum() { return E; }
  bool is_forward() { return IsForward; }
  QualName get_name() override;

protected:
  void emit_impl(std::ostream &SS) override;
};

class TemplateDecl : public Decl {
  Context &C;
  Decl *D;
  std::vector<VarDecl *> Params;

public:
  using iterator = decltype(Params)::iterator;
  TemplateDecl(Context &C, Decl *D, std::vector<VarDecl *> Params)
      : C(C), D(D), Params(Params) {}
  IteratorRange<iterator> params() {
    return IteratorRange<iterator>(Params.begin(), Params.end());
  }
  QualName get_name() override { return D->get_name(); }

protected:
  virtual void emit_impl(std::ostream &SS) override;
};

template <typename DeclT> class TemplateDeclImpl : public TemplateDecl {
protected:
  DeclT *D;

public:
  TemplateDeclImpl(Context &C, DeclT *D, std::vector<VarDecl *> Params)
      : TemplateDecl(C, D, Params), D(D) {}
  DeclT *get_decl() { return D; }
};

class ClassTemplateDecl : public TemplateDeclImpl<ClassDecl> {
public:
  using TemplateDeclImpl::TemplateDeclImpl;
  Class *get_class();
};
class UnionTemplateDecl : public TemplateDeclImpl<UnionDecl> {
public:
  using TemplateDeclImpl::TemplateDeclImpl;
  Union *get_union();
};
class EnumTemplateDecl : public TemplateDeclImpl<EnumDecl> {
public:
  using TemplateDeclImpl::TemplateDeclImpl;
  Enum *get_enum();
};
using FuncTemplateDecl = TemplateDeclImpl<FuncDecl>;
using MethodTemplateDecl = TemplateDeclImpl<MethodDecl>;
using VarTemplateDecl = TemplateDeclImpl<VarDecl>;
using ArrayVarTemplateDecl = TemplateDeclImpl<ArrayVarDecl>;
using VarInitializeTemplateDecl = TemplateDeclImpl<VarInitializeDecl>;
using UsingTemplateDecl = TemplateDeclImpl<UsingDecl>;
using RawTemplateDecl = TemplateDeclImpl<RawDecl>;

} // namespace namecxx

#endif // NAMEC_GENCXX_DECL_H
#undef NAMEC_GENCXX_DECL_H_CYCLIC
