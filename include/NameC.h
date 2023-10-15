#ifndef NAMEC_H
#define NAMEC_H
#include "internal/Gen.h"

/**
  @brief namec is for C code generation.
  @note
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
