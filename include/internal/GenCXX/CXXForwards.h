#ifndef NAMEC_GENCXX_FORWARDS_H
#define NAMEC_GENCXX_FORWARDS_H

#include <functional>
#include <iosfwd>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "internal/Util/IteratorRange.h"
#include "internal/Util/VUIterator.h"

using namespace namec_util;

/**
 @brief namecxx is for C++ code generation.
 */
namespace namecxx {
class CXXFile;
class Context;
class MacroFuncScope;
class Expr;
class Stmt;
class Type;
class ClassOrUnion;
class Class;
class Union;
class Enum;
class Decl;
class VarDecl;
class TopLevel;
class FuncScope;
class ClassTopLevel;
class Directive;
class TypeAlias;
class Attribute;
class QualNameExpr;
class UbiquitousDeclStmtMixin;
class ClassTopLevel;
class ClassMemberDeclMixin;
} // namespace namecxx

#endif
