#include "internal/Gen.h"

#include <fstream>

using namespace namec;

void CFile::emit_to_file(std::filesystem::path Path) {
  std::ofstream OS(Path);
  emit(OS);
}

void CFile::emit_impl(std::ostream &SS) {
  for (auto &T : TopLevels) {
    T->emit(SS);
    SS << "\n";
  }
}
