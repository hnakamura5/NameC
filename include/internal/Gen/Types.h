#ifndef NAMEC_GEN_TYPES_H
#define NAMEC_GEN_TYPES_H

#include "internal/Gen/Decl.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/Mixins.h"

namespace namec {

class Type : public CommentMixin {

public:
  virtual ~Type() = default;
  virtual void emit(std::stringstream &SS) = 0;
};

class RawType : public Type {
  std::string Val;

public:
  RawType(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }
  void emit(std::stringstream &SS) override;
};

class Void : public Type {
public:
  void emit(std::stringstream &SS) override;
};

class Named : public Type {
  Decl *D;

public:
  Named(Decl *D) : D(D) {}
  Decl *get_decl() { return D; }
  void emit(std::stringstream &SS) override;
};

class Pointer : public Type {
  Type *ElmTy;

public:
  Pointer(Type *ElmTy) : ElmTy(ElmTy) {}
  Type *get_elm_type() { return ElmTy; }
  void emit(std::stringstream &SS) override;
};

class Array : public Type {
  Type *ElmTy;
  std::vector<Expr *> Size;

public:
  Array(Type *ElmTy, std::vector<Expr *> Size) : ElmTy(ElmTy), Size(Size) {}
  Type *get_elm_type() { return ElmTy; }
  std::vector<Expr *> get_size() { return Size; }
  void emit(std::stringstream &SS) override;
};

class Function : public Type {
  Type *RetTy;
  std::vector<Type *> Params;

public:
  Function(Type *RetTy, std::vector<Type *> Params)
      : RetTy(RetTy), Params(Params) {}
  Type *get_ret_type() { return RetTy; }
  std::vector<Type *> get_params() { return Params; }
  void emit(std::stringstream &SS) override;
};

class MembersType : public Type {
  std::string Name;
  std::vector<std::unique_ptr<VarDecl>> Members;

public:
  using iterator = VUIterator<VarDecl>;
  MembersType(std::string Name) : Name(Name) {}
  std::string get_name() { return Name; }
  size_t size() { return Members.size(); }
  IteratorRange<iterator> members() {
    return IteratorRange<iterator>(iterator(Members, 0),
                                   iterator(Members, size()));
  }
  void def_member(Type *Ty, std::string Name) {
    Members.push_back(std::make_unique<VarDecl>(Name, Ty));
  }
  void emit(std::stringstream &SS) override;
};

class Struct : public MembersType {
public:
  Struct(std::string Name) : MembersType(Name) {}
  void emit(std::stringstream &SS) override;
};

class Union : public MembersType {
public:
  Union(std::string Name) : MembersType(Name) {}
  void emit(std::stringstream &SS) override;
};

class Enum : public Type {
  std::string Name;
  std::vector<std::pair<std::string, Expr *>> Members;

public:
  using iterator = std::vector<std::pair<std::string, Expr *>>::iterator;
  Enum(std::string Name) : Name(Name) {}
  std::string get_name() { return Name; }
  size_t size() { return Members.size(); }
  IteratorRange<iterator> members() {
    return IteratorRange<iterator>(Members.begin(), Members.end());
  }
  void def_member(std::string Name, Expr *Value) {
    Members.push_back(std::make_pair(Name, Value));
  }
  void emit(std::stringstream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_TYPES_H