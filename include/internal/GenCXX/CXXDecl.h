#ifndef NAMEC_GENCXX_DECL_H
#define NAMEC_GENCXX_DECL_H

#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {
class Decl : public Emit {
  std::vector<Attribute *> Attrs;

public:
  virtual ~Decl() = default;
  virtual void emit_impl(std::ostream &SS) = 0;
  void add_attr(Attribute *Attr) { Attrs.push_back(Attr); }
};

class RawDecl : public Decl {
  std::string Val;

public:
  RawDecl(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class VarDecl : public Decl {
  Type *Ty;
  std::string Name;
  Expr *Init = nullptr;

  bool IsConst = false;
  bool IsExtern = false;
  bool IsStatic = false;
  bool IsVolatile = false;
  bool IsRestrict = false;
  bool IsInline = false;

public:
  VarDecl(std::string Name, Type *Ty) : Ty(Ty), Name(Name) {}
  VarDecl(std::string Name, Type *Ty, Expr *Init)
      : Ty(Ty), Name(Name), Init(Init) {}
  Type *get_type() { return Ty; }
  std::string get_name() { return Name; }
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

protected:
  void emit_impl(std::ostream &SS) override;
};

class ArrayVarDecl : public VarDecl {
  std::vector<Expr *> Size;

public:
  ArrayVarDecl(std::string Name, Type *Ty, std::vector<Expr *> Size)
      : VarDecl(Name, Ty), Size(Size) {}
  ArrayVarDecl(std::string Name, Type *Ty, std::vector<Expr *> Size, Expr *Init)
      : VarDecl(Name, Ty, Init), Size(Size) {}
  std::vector<Expr *> get_size() { return Size; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TypedefDecl : public Decl {
  TypeAlias *TA;

public:
  TypedefDecl(TypeAlias *TA) : TA(TA) {}
  TypeAlias *get_type_alias() { return TA; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class FuncDecl : public Decl {
  Context &C;
  Type *RetTy;
  std::string Name;
  std::vector<VarDecl *> Params;
  // Empty for declaration
  Scope *Body = nullptr;

  bool IsExtern = false;
  bool IsStatic = false;
  bool IsConst = false;
  bool IsOverride = false;
  bool IsVirtual = false;
  bool IsInline = false;

public:
  FuncDecl(Context &C, std::string Name, Type *RetTy,
           std::vector<VarDecl *> Params)
      : C(C), RetTy(RetTy), Name(Name), Params(Params) {}
  Type *get_ret_type() { return RetTy; }
  std::string get_name() { return Name; }
  std::vector<VarDecl *> get_params() { return Params; }
  Scope *get_or_add_body();
  void set_extern(bool IsExtern) { this->IsExtern = IsExtern; }
  bool is_extern() { return IsExtern; }
  void set_static(bool IsStatic) { this->IsStatic = IsStatic; }
  bool is_static() { return IsStatic; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class StructDecl : public Decl {
  Struct *S;

public:
  StructDecl(Struct *S) : S(S) {}
  Struct *get_struct() { return S; }
  std::string get_name();

protected:
  void emit_impl(std::ostream &SS) override;
};

class UnionDecl : public Decl {
  Union *U;

public:
  UnionDecl(Union *U) : U(U) {}
  Union *get_union() { return U; }
  std::string get_name();

protected:
  void emit_impl(std::ostream &SS) override;
};

class EnumDecl : public Decl {
  Enum *E;

public:
  EnumDecl(Enum *E) : E(E) {}
  Enum *get_enum() { return E; }
  std::string get_name();

protected:
  void emit_impl(std::ostream &SS) override;
};

class ClassDecl : public Decl {
  Class *C;

public:
  ClassDecl(Class *C) : C(C) {}
  Class *get_class() { return C; }
  std::string get_name();

protected:
  void emit_impl(std::ostream &SS) override;
};

class EnumClassDecl : public Decl {
  EnumClass *EC;

public:
  EnumClassDecl(EnumClass *EC) : EC(EC) {}
  EnumClass *get_enum_class() { return EC; }
  std::string get_name();

protected:
  void emit_impl(std::ostream &SS) override;
};

class UsingDecl : public Decl {
  std::string Name;
  Type *T;

public:
  UsingDecl(std::string Name, Type *T) : Name(Name), T(T) {}
  std::string get_name() { return Name; }
  Type *get_type() { return T; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TemplateDecl : public Decl {

  Context &C;
  Decl *D;
  std::vector<Decl *> Params;

public:
  using iterator = decltype(Params)::iterator;
  TemplateDecl(Context &C, Decl *D, std::vector<Decl *> Params)
      : C(C), D(D), Params(Params) {}
  Decl *get_decl() { return D; }
  IteratorRange<iterator> params() {
    return IteratorRange<iterator>(Params.begin(), Params.end());
  }

protected:
  virtual void emit_impl(std::ostream &SS) override;
};
} // namespace namecxx

#endif // NAMEC_GENCXX_DECL_H
