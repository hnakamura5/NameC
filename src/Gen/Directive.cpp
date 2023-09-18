#include "internal/Gen.h"

using namespace namec;

void RawDirective::emit_impl(std::ostream &SS) { SS << get_val(); }

void Include::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#include \"";
  SS << get_path();
  SS << "\"\n";
}

void SystemInclude::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#include <";
  SS << get_path();
  SS << ">\n";
}

void Define::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#define ";
  SS << get_name();
  if (!get_value().empty()) {
    SS << " ";
    SS << get_value();
  }
  SS << "\n";
}

DefineFuncMacro::DefineFuncMacro(Context &C, std::string Name,
                                 std::vector<std::string> Args)
    : C(C), Name(Name), Args(Args) {
  Body.reset(new MacroFuncScope(C));
}

void DefineFuncMacro::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#define ";
  SS << get_name();
  SS << "(";
  for (auto I = 0; I < Args.size(); ++I) {
    SS << Args[I];
    if (I != Args.size() - 1) {
      SS << ",";
    }
  }
  SS << ") ";
  get_body()->emit(SS);
  SS << "\n";
}

void Undef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#undef ";
  SS << get_name();
  SS << "\n";
}

void Pragma::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#pragma ";
  SS << get_value();
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
  if (!Else.get()) {
    Else.reset(new TopLevel(C));
  }
  return Else.get();
}

void IfDirectiveBase::emit_impl(std::ostream &SS) {
  // Other part than #if, #ifdef, #ifndef
  SS << "\n";
  get_then()->emit(SS);
  for (auto &[ElifCond, ElifThen] : Elifs) {
    SS << "\n";
    SS << "#elif ";
    ElifCond->emit(SS);
    SS << "\n";
    ElifThen->emit(SS);
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
  SS << "#if ";
  get_cond()->emit(SS);
  IfDirectiveBase::emit_impl(SS);
}

void Ifdef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#ifdef ";
  SS << get_cond();
  IfDirectiveBase::emit_impl(SS);
}

void Ifndef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#ifndef ";
  SS << get_cond();
  IfDirectiveBase::emit_impl(SS);
}
