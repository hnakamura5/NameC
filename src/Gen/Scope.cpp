#include "internal/Gen.h"

using namespace namec;

void Scope::emit_impl(std::stringstream &SS) {
  for (auto *E : Entries) {
    E->emit(SS);
    SS << "\n";
  }
}
