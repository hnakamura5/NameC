#include "internal/Gen.h"

using namespace namec;

void Scope::emit_impl(std::ostream &SS) {
  for (auto *E : Entries) {
    E->emit(SS);
  }
}

void MacroFuncScope::emit_impl(std::ostream &SS) {
  std::stringstream LocalSS;
  for (auto *E : Entries) {
    E->emit(LocalSS);
  }
  std::string LocalStr = LocalSS.str();
  // Replace all newline with newline in macro
  size_t Pos = 0;
  while ((Pos = LocalStr.find("\n", Pos)) != std::string::npos) {
    LocalStr.replace(Pos, 1, "\\\n");
    Pos += 2;
  }
  SS << LocalStr;
}
