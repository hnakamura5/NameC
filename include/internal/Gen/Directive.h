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

class IfDirective : public Directive {
  Context &C;

protected:
  std::string Cond;
  std::unique_ptr<TopLevel> Then;
  std::unique_ptr<TopLevel> Else;

public:
  IfDirective(Context &C, std::string Cond);
  virtual ~IfDirective() = default;
  std::string get_cond() { return Cond; }
  TopLevel *get_then() { return Then.get(); }
  bool has_else() { return Else.get() != nullptr; }
  TopLevel *get_or_add_else();

protected:
  void emit_impl(std::ostream &SS) override;
};

class Ifdef : public IfDirective {

public:
  Ifdef(Context &C, std::string Cond) : IfDirective(C, Cond) {}

protected:
  void emit_impl(std::ostream &SS) override;
};

class Ifndef : public IfDirective {

public:
  Ifndef(Context &C, std::string Cond) : IfDirective(C, Cond) {}

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_DIRECTIVE_H
