#ifndef NAMEC_H
#define NAMEC_H
#include "internal/Gen.h"

/**
  @brief namec is for C code generation.

  ## Important classes

  ### Context

  The class Context is the main factory class for expr and types.
  It also have their ownership and manages their lifetime.

  ### CFile

  CFile corresponds to a C source or header file. Constructed using Context
  object. It contains top-level declarations and directives.

  ### TopLevel

  TopLevel is file top-level scope. This has factory methods and contains
  directives and declarations of function and types.

  ### FuncScope, MacroFuncScope

  FuncScope is a scope of a function. It has factories for and contains
  control statements, declarations and directives. Expressions are created using
  Context object and passed to them. MacroFuncScope cannot contain directives.

  ## Examples

  Some examples here. For more examples, visit the source code and see the
  tests.

  ### Emitting file with main function
  ```cpp
  #include "NameC.h"
  #include <fstream>

  int main()  {
    Context C;
    CFile F(C);

    TopLevel *T = F.get_first_top_level();
    FuncDecl *Main =
      T->def_func("main", C.type_int(), {C.decl_var("", C.type_void())});
    Scope *S = Main->get_or_add_body();
    S->stmt_return(C.expr_int(0));

    std::filesystem::path P = "test.c";
    F.emit_to_file(P);
  }
  ```
  then the file test.c contains the following code:
  ```c
  int main(void){return 0;}
  ```
 */
namespace namec {
// Only for doxygen documentation
} // namespace namec

#endif // NAMEC_H
