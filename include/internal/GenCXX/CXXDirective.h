#ifdef NAMEC_GENCXX_DIRECTIVE_H_CYCLIC
static_assert(false, "Cyclic include detected of " __FILE__);
#endif
#define NAMEC_GENCXX_DIRECTIVE_H_CYCLIC
#ifndef NAMEC_GENCXX_DIRECTIVE_H
#define NAMEC_GENCXX_DIRECTIVE_H

#include "internal/GenCXX/CXXCommon.h"
#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {

class Directive : public Emit {

public:
  virtual ~Directive() {}
  virtual void emit_impl(std::ostream &SS) = 0;
};

class Include : public Directive {
  std::string Path;

public:
  Include(std::string Path) : Path(Path) {}
  std::string get_path() { return Path; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class RawDirective : public Directive {
  std::string Val;

public:
  RawDirective(std::string Val) : Val(Val) {}
  std::string get_val() { return Val; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class SystemInclude : public Directive {
  std::string Path;

public:
  SystemInclude(std::string Path) : Path(Path) {}
  std::string get_path() { return Path; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Define : public Directive {
  std::string Name;
  // TODO: Only raw string value? We want more support for defining macro
  // function.
  std::string Value;

public:
  Define(std::string Name, std::string Value) : Name(Name), Value(Value) {}
  std::string get_name() { return Name; }
  std::string get_value() { return Value; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class DefineFuncMacro : public Directive {
  Context &C;
  std::string Name;
  std::vector<std::string> Args;
  std::unique_ptr<MacroFuncScope> Body;

public:
  using iterator = decltype(Args)::iterator;
  DefineFuncMacro(Context &C, std::string Name, std::vector<std::string> Args);
  std::string get_name() { return Name; }
  IteratorRange<iterator> args() {
    return IteratorRange<iterator>(Args.begin(), Args.end());
  }
  MacroFuncScope *get_body() { return Body.get(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Undef : public Directive {
  std::string Name;

public:
  Undef(std::string Name) : Name(Name) {}
  std::string get_name() { return Name; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Pragma : public Directive {
  // TODO: Only raw string value?
  std::string Value;

public:
  Pragma(std::string Value) : Value(Value) {}
  std::string get_value() { return Value; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class IfDirectiveBase : public Directive {
  Context &C;

protected:
  TopLevel *Then;
  std::vector<std::pair<Expr *, TopLevel *>> Elifs;
  TopLevel *Else;

  // TODO: elif

public:
  IfDirectiveBase(Context &C) : C(C), Then(C.add_top_level()) {}
  TopLevel *get_then() { return Then; }
  TopLevel *add_elif(Expr *Cond);
  bool has_else() { return Else != nullptr; }
  TopLevel *get_or_add_else();

protected:
  void emit_impl(std::ostream &SS) override;
};

class IfDirective : public IfDirectiveBase {

protected:
  Expr *Cond;

public:
  IfDirective(Context &C, Expr *Cond);
  virtual ~IfDirective() = default;
  Expr *get_cond() { return Cond; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Ifdef : public IfDirectiveBase {
  std::string CondStr;

public:
  Ifdef(Context &C, std::string Cond) : IfDirectiveBase(C), CondStr(Cond) {}
  std::string get_cond() { return CondStr; }

protected:
  void emit_impl(std::ostream &SS) override;
};

class Ifndef : public IfDirectiveBase {
  std::string CondStr;

public:
  Ifndef(Context &C, std::string Cond) : IfDirectiveBase(C), CondStr(Cond) {}
  std::string get_cond() { return CondStr; }

protected:
  void emit_impl(std::ostream &SS) override;
};

// namespace Name {Body}
class Namespace : public Directive {
  QualName Name;
  TopLevel *Body;

public:
  Namespace(Context &C, QualName Name) : Name(Name), Body(C.add_top_level()) {}
  QualName get_name() { return Name; }
  std::string get_name_str() { return Name.to_string(); }
  TopLevel *get_body() { return Body; }

protected:
  void emit_impl(std::ostream &SS) override;
};

// using namespace QualName;
class UsingNamespace : public Directive {
  QualName Name;

public:
  UsingNamespace(QualName Name) : Name(Name) {}
  QualName get_name() { return Name; }
  std::string get_name_str() { return Name.to_string(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

// using QualName; (spot name importing to current scope)
class UsingDirective : public Directive {
  QualName Name;

public:
  UsingDirective(QualName Name) : Name(Name) {}
  QualName get_name() { return Name; }
  std::string get_name_str() { return Name.to_string(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_DIRECTIVE_H
#undef NAMEC_GENCXX_DIRECTIVE_H_CYCLIC
