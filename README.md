# NameC

C/C++ code generator library

## Concept

Our goal is to provide simple and handy APIs enough to generate C/C++ code easily from your application.
Note that this library does not aim at supporting complete and sound APIs for C/C++ syntax.

## Installation by CMake

- Build and installation is by standard CMake workflow.
- You need C++ compiler that supports C++17.
- The documents are generated when CMake find doxygen (Sure when in your path env).

## Getting started

- Link the library file lib/NameC.lib from the installation.
- Include the header include/NameC.h for C code generation. It provides the namespace namec.
- Include the header include/NameCXX.h for C++ code generation. It provides the namespace namecxx.
- Read the doxygen documentation in doc directory in the release build. Top pages of each namespace works as index.

## Example

```cpp
#include "NameC.h"
#include <iostream>

using namespace namec;

int main(int argc, char *argv[]) {
  Context C;
  CFile F(C);
  TopLevel *T = F.get_first_top_level();
  T->include_sys("stdio.h");
  VarDecl *ArgC = C.decl_var("argc", C.type_int());
  VarDecl *ArgV = C.decl_array_var("argv", C.type_ptr(C.type_char()), {});
  FuncScope *Main =
      T->def_func("main", C.type_int(), {ArgC, ArgV})->get_or_add_body();
  IfStmt *If = Main->stmt_if(C.EX(C.EX(ArgC), ">", C.EX(1)));
  FuncScope *Then = If->get_then();
  FuncScope *Else = If->get_or_add_else();
  Then->stmt_call(C.EX("printf"), {C.STR("Hello, %s!\\n"), C.IDX(ArgV, 1)});
  Else->stmt_call(C.EX("printf"), {C.STR("Hello, world!\\n")});
  Main->stmt_return(C.EX(0));
  F.emit_to_file("hello.c");
  F.emit(std::cout);
  return 0;
}
```

This generates (and prints) the following C code.

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (((argc) > (1))) {
    printf("Hello, %s!\n", (argv[1]));
  } else {
    printf("Hello, world!\n");
  }
  return 0;
}

```

Note that this is formatted by clang-format LLVM style. The raw output is

```c
#include <stdio.h>

int main(int argc,char* argv[]){if(((argc)>(1))){printf("Hello, %s!\n",(argv[1]));}else{printf("Hello, world!\n");}return 0;}
```

You can easily see that it is easy to write the program yourself.
This library may work well for generating program with very complex structures.
