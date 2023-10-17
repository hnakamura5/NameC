#ifndef NAMECXX_H
#define NAMECXX_H
#include "internal/GenCXX.h"

/**
  @brief namecxx is for C++ code generation.

## Important classes

  The architecture is almost same as namec.

  ### Context

  The class Context is the main factory class for expr and types.
  It also have their ownership and manages their lifetime.

  ### CXXFile

  CXXFile corresponds to a C++ source or header file. Constructed using Context
  object. It contains top-level declarations and directives.

  ### TopLevel

  TopLevel is file top-level scope. This has factory methods and contains
  directives and declarations of function and types.

  ### FuncScope, MacroFuncScope

  FuncScope is a scope of a function. It has factories for and contains
  control statements, declarations and directives. Expressions are created using
  Context object and passed to them. MacroFuncScope cannot contain directives.

  ### ClassTopLevel

  ClassTopLevel is a top-level scope of a class. It has factories for methods,
fields, inner classes and so on. Note that here we cannot define namespaces.
  In a class there are many ClassTopLevel, each of them are with their access
controls. The namecxx::Class class itself does not have member factory method.
This is different point from namec::Struct.

  ## Examples

  Some examples here. For more examples, visit the source code and see the
  tests.
  */

namespace namecxx {
// Only for doxygen documentation
} // namespace namecxx

#endif // NAMECXX_H
