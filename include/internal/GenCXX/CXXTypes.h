#ifdef NAMEC_GENCXX_TYPES_H_CYCLIC
static_assert(false, "Cyclic include detected of " __FILE__);
#endif
#define NAMEC_GENCXX_TYPES_H_CYCLIC
#ifndef NAMEC_GENCXX_TYPES_H
#define NAMEC_GENCXX_TYPES_H

#include "internal/GenCXX/CXXCommon.h"
#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {

class Type : public TypeOrExpr {

public:
  virtual ~Type() = default;
  virtual void emit_impl(std::ostream &SS) = 0;
};

class RawType : public Type {
  std::string Val;

public:
  RawType(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Void : public Type {
public:
protected:
  void emit_impl(std::ostream &SS) override;
};

class Named : public Type {
  Decl *D;

public:
  Named(Decl *D) : D(D) {}
  Decl *get_decl() { return D; }
  QualName get_name() { return D->get_name(); }
  std::string get_name_str() { return D->get_name_str(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

class TypeAlias : public Type {
  QualName Name;
  Type *Ty;

public:
  TypeAlias(QualName Name, Type *Ty) : Name(Name), Ty(Ty) {}
  QualName get_name() { return Name; }
  std::string get_name_str() { return Name.to_string(); }
  Type *get_type() { return Ty; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Pointer : public Type {
  Type *ElmTy;

public:
  Pointer(Type *ElmTy) : ElmTy(ElmTy) {}
  Type *get_elm_type() { return ElmTy; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Reference : public Type {
  Type *ElmTy;

public:
  Reference(Type *ElmTy) : ElmTy(ElmTy) {}
  Type *get_elm_type() { return ElmTy; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Array : public Type {
  Type *ElmTy;
  std::vector<Expr *> Size;

public:
  using iterator = decltype(Size)::iterator;
  Array(Type *ElmTy, std::vector<Expr *> Size) : ElmTy(ElmTy), Size(Size) {}
  Type *get_elm_type() { return ElmTy; }
  IteratorRange<iterator> size() {
    return IteratorRange<iterator>(Size.begin(), Size.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Function : public Type {
  Type *RetTy;
  std::vector<Type *> Params;
  bool IsVarArg;
  // TODO: throw specifier?

public:
  using param_iterator = decltype(Params)::iterator;
  Function(Type *RetTy, std::vector<Type *> Params, bool IsVarArg = false)
      : RetTy(RetTy), Params(Params), IsVarArg(IsVarArg) {}
  Type *get_ret_type() { return RetTy; }
  std::vector<Type *> get_params() { return Params; }
  IteratorRange<param_iterator> params() {
    return IteratorRange<param_iterator>(Params.begin(), Params.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

struct AccessSpec {
  enum { Public, Protected, Private } Value;
  AccessSpec(decltype(Value) V) : Value(V) {}
  std::string get_name() {
    switch (Value) {
    case Public:
      return "public";
    case Protected:
      return "protected";
    case Private:
      return "private";
    }
    return "";
  }
};

class ClassOrUnion : public Type {
  Context &C;
  QualName Name;
  // This have each segment with visibility as top-levels.
  // All members are defined in them.
  std::vector<std::pair<AccessSpec, ClassTopLevel *>> VisibilityScopes;

  ClassTopLevel *add_visibility_scope(AccessSpec AS);

public:
  using iterator = decltype(VisibilityScopes)::iterator;
  ClassOrUnion(Context &C, QualName Name) : C(C), Name(Name) {}
  QualName get_name() { return Name; }
  std::string get_name_str() { return Name.to_string(); }
  ClassTopLevel *add_public_scope() {
    return add_visibility_scope(AccessSpec::Public);
  }
  ClassTopLevel *add_protected_scope() {
    return add_visibility_scope(AccessSpec::Protected);
  }
  ClassTopLevel *add_private_scope() {
    return add_visibility_scope(AccessSpec::Private);
  }
  IteratorRange<iterator> visibility_scopes() {
    return IteratorRange<iterator>(VisibilityScopes.begin(),
                                   VisibilityScopes.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Class : public ClassOrUnion {
  bool IsStruct; // Whether to use struct keyword.
  std::vector<std::pair<AccessSpec, Type *>> Bases;
  bool IsFinal = false;

public:
  using base_iterator = decltype(Bases)::iterator;
  Class(Context &C, QualName Name, decltype(Bases) Bases, bool IsStruct)
      : ClassOrUnion(C, Name), Bases(Bases), IsStruct(IsStruct) {}
  bool is_struct() { return IsStruct; }
  size_t base_size() { return Bases.size(); }
  IteratorRange<base_iterator> bases() {
    return IteratorRange<base_iterator>(Bases.begin(), Bases.end());
  }
  void add_base(AccessSpec AS, Type *Ty) { Bases.push_back({AS, Ty}); }
  bool is_final() { return IsFinal; }
  void set_final(bool IsFinal) { this->IsFinal = IsFinal; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Union : public ClassOrUnion {
public:
  Union(Context &C, QualName Name) : ClassOrUnion(C, Name) {}

protected:
  void emit_impl(std::ostream &SS) override;
};

class Enum : public Type {
  QualName Name;
  Type *BaseType; // nullptr for non-specifying.
  std::vector<std::pair<QualName, Expr *>> Members;

public:
  using iterator = decltype(Members)::iterator;
  Enum(QualName Name, Type *BaseType) : Name(Name), BaseType(BaseType) {}
  QualName get_name() { return Name; }
  std::string get_name_str() { return Name.to_string(); }
  Type *get_base_type() { return BaseType; }
  size_t size() { return Members.size(); }
  IteratorRange<iterator> members() {
    return IteratorRange<iterator>(Members.begin(), Members.end());
  }
  void def_member(QualName Name, Expr *Value) {
    Members.push_back(std::make_pair(Name, Value));
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class EnumClass : public Enum {
  bool IsStruct; // Whether to use struct keyword.

public:
  EnumClass(QualName Name, Type *BaseType, bool IsStruct)
      : Enum(Name, BaseType) {}
  bool is_struct() { return IsStruct; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Instantiation : public Type {
  TemplateDecl *TD;
  std::vector<TypeOrExpr *> Args;

public:
  using arg_iterator = decltype(Args)::iterator;
  Instantiation(TemplateDecl *TD, std::vector<TypeOrExpr *> Args)
      : TD(TD), Args(Args) {}
  TemplateDecl *get_template_decl() { return TD; }
  IteratorRange<arg_iterator> args() {
    return IteratorRange<arg_iterator>(Args.begin(), Args.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

class PackedType : public Type {
  Type *T;

public:
  PackedType(Type *T) : T(T) {}
  Type *get_type() { return T; }

protected:
  void emit_impl(std::ostream &SS) override;
};

// For template type parameter.
class TypeNameType : public Type {
  bool IsClass; // Whether to use class keyword.

public:
  TypeNameType(bool IsClass) : IsClass(IsClass) {}
  bool is_class() { return IsClass; }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_TYPES_H
#undef NAMEC_GENCXX_TYPES_H_CYCLIC
