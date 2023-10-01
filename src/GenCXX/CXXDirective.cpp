#include "internal/GenCXX.h"

using namespace namecxx;

void RawDirective::emit_impl(std::ostream &SS) { SS << get_val(); }

void Include::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#include \"" << get_path();
  SS << "\"\n";
}

void SystemInclude::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#include <" << get_path();
  SS << ">\n";
}

void Define::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#define " << get_name();
  if (!get_value().empty()) {
    SS << " " << get_value();
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
  SS << "#define " << get_name() << "(" << join(args()) << ") ";
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

TopLevel *IfDirectiveBase::add_elif(Expr *Cond) {
  auto *Ret = C.add_top_level();
  Elifs.push_back({Cond, Ret});
  return Ret;
}

TopLevel *IfDirectiveBase::get_or_add_else() {
  if (!Else) {
    Else = C.add_top_level();
  }
  return Else;
}

void IfDirectiveBase::emit_impl(std::ostream &SS) {
  // Other part than #if, #ifdef, #ifndef
  SS << "\n";
  SS << get_then();
  for (auto &[ElifCond, ElifThen] : Elifs) {
    SS << "\n";
    SS << "#elif " << ElifCond;
    SS << "\n";
    SS << ElifThen;
  }
  SS << "\n";
  if (has_else()) {
    SS << "#else\n";
    SS << Else;
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

void Namespace::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "namespace " << get_name_str() << "{";
  SS << get_body();
  SS << "}";
}

void UsingNamespace::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "using namespace " << get_name_str() << ";";
}

void UsingDirective::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "using " << get_name_str() << ";";
}
