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

// TODO: function style macro
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

IfDirective::IfDirective(Context &C, std::string Cond) : C(C), Cond(Cond) {
  Then.reset(new TopLevel(C));
}

TopLevel *IfDirective::get_or_add_else() {
  if (!Else.get()) {
    Else.reset(new TopLevel(C));
  }
  return Else.get();
}

void IfDirective::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#if ";
  SS << get_cond();
  SS << "\n";
  get_then()->emit(SS);
  SS << "\n";
  if (has_else()) {
    SS << "#else\n";
    Else->emit(SS);
    SS << "\n";
  }
  SS << "#endif\n";
}

void Ifdef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#ifdef ";
  SS << get_cond();
  SS << "\n";
  get_then()->emit(SS);
  SS << "\n";
  if (has_else()) {
    SS << "#else\n";
    Else->emit(SS);
    SS << "\n";
  }
  SS << "#endif\n";
}

void Ifndef::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#ifndef ";
  SS << get_cond();
  SS << "\n";
  get_then()->emit(SS);
  SS << "\n";
  if (has_else()) {
    SS << "#else\n";
    Else->emit(SS);
    SS << "\n";
  }
  SS << "#endif\n";
}
