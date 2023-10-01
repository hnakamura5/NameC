#include "internal/GenCXX.h"

#include <fstream>

using namespace namecxx;

CXXFile::CXXFile(Context &C) : C(C) { TopLevels.push_back(C.add_top_level()); }

TopLevel *CXXFile::add_top_level() {
  TopLevels.push_back(C.add_top_level());
  return TopLevels.back();
}

void CXXFile::emit_to_file(std::filesystem::path Path) {
  std::ofstream OS(Path);
  emit(OS);
}

void CXXFile::emit_impl(std::ostream &SS) {
  for (auto *T : TopLevels) {
    SS << T << "\n";
  }
}
