#ifndef NAMEC_GEN_DIRECTIVE_H
#define NAMEC_GEN_DIRECTIVE_H

#include "internal/Gen/Emit.h"
#include "internal/Gen/Forwards.h"

namespace namec {

class Directive : public Emit {

public:
  virtual ~Directive() = default;
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
  std::unique_ptr<TopLevel> Then;
  std::vector<std::pair<Expr *, std::unique_ptr<TopLevel>>> Elifs;
  std::unique_ptr<TopLevel> Else;

  // TODO: elif

public:
  IfDirectiveBase(Context &C);
  TopLevel *get_then() { return Then.get(); }
  TopLevel *add_elif(Expr *Cond);
  bool has_else() { return Else.get() != nullptr; }
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

} // namespace namec

#endif // NAMEC_GEN_DIRECTIVE_H
