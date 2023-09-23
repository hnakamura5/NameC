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
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Stmts.h"

namespace namec {

// File TopLevel is split by if directives
class TopLevel : public Emit,
                 public DirectiveDefineMixin,
                 public UbiquitousDeclStmtMixIn {
  Context &C;
  std::vector<Emit *> Entries;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  TopLevel(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixIn(C) {}
  // Only in top level we can define/declare functions
  FuncDecl *def_func(std::string Name, Type *RetTy,
                     std::vector<VarDecl *> Params, bool IsVarArg = false);

protected:
  void emit_impl(std::ostream &SS) override;
};

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
