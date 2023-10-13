#include "internal/Gen.h"

using namespace namec;

void RawDirective::emit_impl(std::ostream &SS) { SS << get_val(); }

void Include::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#include \"" << get_path() << "\"";
  SS << "\n";
}

void SystemInclude::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#include <" << get_path() << ">";
  SS << "\n";
}

void Define::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#define " << get_name();
  if (!get_value().empty()) {
    SS << " ";
    SS << get_value();
  }
  SS << "\n";
}

DefineFuncMacro::DefineFuncMacro(Context &C, std::string Name,
                                 std::vector<std::string> Args, bool IsVarArg)
    : C(C), Name(Name), Args(Args), IsVarArg(IsVarArg) {
  Body.reset(new MacroFuncScope(C));
}

void DefineFuncMacro::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#define " << get_name() << "(" << join(args());
  if (is_vararg()) {
    SS << ", ...";
  }
  SS << ") ";
  SS << get_body();
  SS << "\n";
}

void Undef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#undef " << get_name();
  SS << "\n";
}

void Pragma::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#pragma " << get_value();
  SS << "\n";
}

IfDirectiveBase::IfDirectiveBase(Context &C) : C(C) {
  Then.reset(new TopLevel(C));
}

TopLevel *IfDirectiveBase::add_elif(Expr *Cond) {
  auto E = std::make_unique<TopLevel>(C);
  auto *Ret = E.get();
  Elifs.push_back({Cond, std::move(E)});
  return Ret;
}

TopLevel *IfDirectiveBase::get_or_add_else() {
  if (!Else) {
    Else.reset(new TopLevel(C));
  }
  return Else.get();
}

void IfDirectiveBase::emit_impl(std::ostream &SS) {
  // Other part than #if, #ifdef, #ifndef
  SS << "\n";
  SS << get_then();
  for (auto &[ElifCond, ElifThen] : Elifs) {
    SS << "\n";
    SS << "#elif " << ElifCond << "\n";
    SS << ElifThen;
  }
  SS << "\n";
  if (has_else()) {
    SS << "#else\n";
    Else->emit(SS);
    SS << "\n";
  }
  SS << "#endif\n";
}

IfDirective::IfDirective(Context &C, Expr *Cond)
    : IfDirectiveBase(C), Cond(Cond) {}

void IfDirective::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#if " << get_cond();
  IfDirectiveBase::emit_impl(SS);
}

void Ifdef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#ifdef " << get_cond();
  IfDirectiveBase::emit_impl(SS);
}

void Ifndef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#ifndef " << get_cond();
  IfDirectiveBase::emit_impl(SS);
}
