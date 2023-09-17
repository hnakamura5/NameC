#ifndef NAMEC_GEN_FILE_H
#define NAMEC_GEN_FILE_H

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
  TopLevel(Context &C) : C(C), UbiquitousDeclStmtMixIn(C) {}
  // Only in top level we can define/declare functions
  FuncDecl *def_func(std::string Name, Type *RetTy,
                     std::vector<VarDecl *> Params);

protected:
  void emit_impl(std::ostream &SS) override;
};

class File : public Emit {

  Context &C;
  std::unique_ptr<TopLevel> TheTopLevel;

public:
  File(Context &C) : C(C) { TheTopLevel.reset(new TopLevel(C)); }
  virtual ~File() {}
  TopLevel *get_top_level() { return TheTopLevel.get(); }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namec

#endif
