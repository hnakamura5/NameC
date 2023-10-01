#ifdef NAME_GENCXX_FILE_H_CYCLIC
static_assert(false, "Cyclic include detected of " __FILE__);
#endif
#define NAME_GENCXX_FILE_H_CYCLIC

#ifndef NAME_GENCXX_FILE_H
#define NAME_GENCXX_FILE_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "internal/GenCXX/CXXCommon.h"
#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {

class CXXFile : public Emit {

  Context &C;
  std::vector<TopLevel *> TopLevels;

public:
  CXXFile(Context &C);
  virtual ~CXXFile() {}
  TopLevel *get_first_top_level() { return TopLevels[0]; }
  // Add a new top level. This is only for convenience of generation.
  TopLevel *add_top_level();
  void emit_to_file(std::filesystem::path Path);

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAME_GENCXX_FILE_H
#undef NAME_GENCXX_FILE_H_CYCLIC
