#ifndef NAMEC_GEN_SCOPE_H
#define NAMEC_GEN_SCOPE_H

#include "internal/Gen/Directive.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/File.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Stmts.h"

namespace namec {
// Scope is for normal scope, such as function body, if body, etc.
class Scope : public Emit,
              public DirectiveDefineMixin,
              public UbiquitousDeclStmtMixIn,
              public InFunctionStmtMixIn {
  Context &C;

  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Stmt>> Stmts;

protected:
  virtual void on_add_directive(Directive *D) override { Entries.push_back(D); }
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  virtual void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  Scope(Context &C)
      : C(C), DirectiveDefineMixin(C), UbiquitousDeclStmtMixIn(C),
        InFunctionStmtMixIn(C) {}
  virtual ~Scope() {}
  void emit_impl(std::ostream &SS) override;
};

// MacroFuncScope is only as the toplevel scope of a macro function
// Here directives are not allowed.
class MacroFuncScope : public Emit,
                       public UbiquitousDeclStmtMixIn,
                       public InFunctionStmtMixIn {
  Context &C;
  std::vector<Emit *> Entries;
  std::vector<std::unique_ptr<Stmt>> Stmts;

protected:
  virtual void on_add_decl_stmt(Stmt *S) override { Entries.push_back(S); }
  virtual void on_add_stmt(Stmt *S) override { Entries.push_back(S); }

public:
  MacroFuncScope(Context &C)
      : C(C), UbiquitousDeclStmtMixIn(C), InFunctionStmtMixIn(C) {}
  virtual ~MacroFuncScope() {}
  void emit_impl(std::ostream &SS) override;
};

} // namespace namec

#endif // NAMEC_GEN_SCOPE_H
