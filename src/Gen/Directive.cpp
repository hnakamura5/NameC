#include "internal/Gen.h"

using namespace namec;

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
  SS << " ";
  SS << get_value();
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

void IfDirective::emit_impl(std::ostream &SS) {
  SS << "\n";
  SS << "#if ";
  SS << get_cond();
  SS << "\n";
  get_then()->emit(SS);
  SS << "\n";
  if (get_else()) {
    SS << "#else\n";
    get_else()->emit(SS);
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
  if (get_else()) {
    SS << "#else\n";
    get_else()->emit(SS);
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
  if (get_else()) {
    SS << "#else\n";
    get_else()->emit(SS);
    SS << "\n";
  }
  SS << "#endif\n";
}
