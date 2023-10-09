#ifndef NAMEC_GEN_FILE_H
#define NAMEC_GEN_FILE_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "internal/Gen/Decl.h"
#include "internal/Gen/Directive.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/Mixins.h"
#include "internal/Gen/Stmts.h"

namespace namec {

class CFile : public Emit {

  Context &C;
  std::vector<std::unique_ptr<TopLevel>> TopLevels;

public:
  CFile(Context &C) : C(C) {
    TopLevels.push_back(std::make_unique<TopLevel>(C));
  }
  virtual ~CFile() {}
  TopLevel *get_first_top_level() { return TopLevels[0].get(); }
  // Add a new top level. This is only for convenience of generation.
  TopLevel *add_top_level() {
    TopLevels.push_back(std::make_unique<TopLevel>(C));
    return TopLevels.back().get();
  }
  void emit_to_file(std::filesystem::path Path);

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namec

#endif
