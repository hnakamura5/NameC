#ifndef NAMEC_GEN_DECL_H
#define NAMEC_GEN_DECL_H

#include "internal/Gen/Emit.h"
#include "internal/Gen/Forwards.h"

namespace namec {

class Decl : public Emit {
public:
  virtual ~Decl() = default;
  virtual void emit_impl(std::ostream &SS) = 0;
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
  Type *RetTy;
  std::string Name;
  std::vector<VarDecl *> Params;
  // Empty for declaration
  Scope *Body = nullptr;

  bool IsExtern = false;
  bool IsStatic = false;

public:
  FuncDecl(std::string Name, Type *RetTy, std::vector<VarDecl *> Params)
      : RetTy(RetTy), Name(Name), Params(Params) {}
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

} // namespace namec

#endif // NAMEC_GEN_DECL_H
