#ifndef NAMEC_GEN_DIRECTIVE_H
#define NAMEC_GEN_DIRECTIVE_H

#include "internal/Gen/Emit.h"
#include "internal/Gen/Forwards.h"

namespace namec {

class Directive : public Emit {

public:
  virtual ~Directive() = default;
  virtual void emit(std::stringstream &SS) = 0;
};

class Include : public Directive {
  std::string Path;

public:
  Include(std::string Path) : Path(Path) {}
  std::string get_path() { return Path; }
  void emit(std::stringstream &SS) override;
};

class SystemInclude : public Directive {
  std::string Path;

public:
  SystemInclude(std::string Path) : Path(Path) {}
  std::string get_path() { return Path; }
  void emit(std::stringstream &SS) override;
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
  void emit(std::stringstream &SS) override;
};

class Undef : public Directive {
  std::string Name;

public:
  Undef(std::string Name) : Name(Name) {}
  std::string get_name() { return Name; }
  void emit(std::stringstream &SS) override;
};

class Pragma : public Directive {
  // TODO: Only raw string value?
  std::string Value;

public:
  Pragma(std::string Value) : Value(Value) {}
  std::string get_value() { return Value; }
  void emit(std::stringstream &SS) override;
};

class IfDirective : public Directive {
protected:
  std::string Cond;
  std::unique_ptr<TopLevel> Then;
  std::unique_ptr<TopLevel> Else;

public:
  IfDirective(std::string Cond);
  std::string get_condition() { return Cond; }
  TopLevel *get_then();
  TopLevel *get_or_add_else();
  void emit(std::stringstream &SS) override;
};

class IfdefDirective : public IfDirective {

public:
  IfdefDirective(std::string Cond) : IfDirective(Cond) {}
  void emit(std::stringstream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_DIRECTIVE_H