#ifndef NAMEC_GEN_FILE_H
#define NAMEC_GEN_FILE_H

#include <memory>
#include <string>
#include <vector>

#include "internal/Gen/Directive.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"

namespace namec {

// File TopLevel is split by if directives
class TopLevel : public Emit,
                 public DirectiveDefineMixin,
                 public UbiquitousDeclStmtMixIn {
  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<TopLevel>> Children;

protected:
  void on_add_directive(Directive *D) override { Entries.push_back(D); }
  void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  TopLevel(Context &C) : UbiquitousDeclStmtMixIn(C) {}
  TopLevel *add_get_new_top_level() {
    Children.push_back(std::make_unique<TopLevel>());
    return Children.back().get();
  }
  // Only in top level we can define/declare functions
  FuncDecl *def_func(std::string Name, Type *RetTy,
                     std::vector<VarDecl *> Params);
  void emit(std::stringstream &SS) override;
};

class File : public Emit {

  Context &C;
  std::unique_ptr<TopLevel> TheTopLevel;

public:
  File(Context &C) : C(C) { TheTopLevel.reset(new TopLevel(C)); }
  virtual ~File() {}
  TopLevel *get_top_level() { return TheTopLevel.get(); }
  void emit(std::stringstream &SS) override;
};

} // namespace namec

#endif
