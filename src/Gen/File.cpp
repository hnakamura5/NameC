#include "internal/Gen.h"

using namespace namec;

namespace {

struct TopLevel {
  virtual void emit(std::stringstream &SS);
};

struct Include : public TopLevel {
  bool IsSystem;
  std::string Path;

  void emit(std::stringstream &SS) override;
};

struct Define : public TopLevel {
  std::string Name;
  std::vector<std::string> Args;
  std::optional<std::string> Value;

  void emit(std::stringstream &SS) override;
};

struct Undef : public TopLevel {
  std::string Name;

  void emit(std::stringstream &SS) override;
};

struct IfDirective : public TopLevel {
  std::string Condition;
  std::vector<std::unique_ptr<TopLevel>> Then;
  std::vector<std::unique_ptr<TopLevel>> Else;

  void emit(std::stringstream &SS) override;
};

struct Struct : public TopLevel {
  std::string Name;
  std::vector<std::pair<Type *, std::string>> Members;

  void emit(std::stringstream &SS) override;
};

struct Enum : public TopLevel {
  std::string Name;
  std::vector<std::pair<std::string, std::optional<Expr *>>> Members;

  void emit(std::stringstream &SS) override;
};

struct Union : public TopLevel {
  std::string Name;
  std::vector<std::pair<Type *, std::string>> Members;

  void emit(std::stringstream &SS) override;
};

struct Function : public TopLevel {
  std::string Name;
  std::vector<std::pair<Type *, std::string>> Args;
  std::optional<Type *> RetType;
  std::unique_ptr<Scope> Body;

  void emit(std::stringstream &SS) override;
};

struct TypeDef : public TopLevel {
  std::unique_ptr<Type> Defined;
  Type *Type;

  void emit(std::stringstream &SS) override;
};

} // namespace
