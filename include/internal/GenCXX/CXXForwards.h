#ifndef NAMEC_GENCXX_FORWARDS_H
#define NAMEC_GENCXX_FORWARDS_H

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

namespace namecxx {
class CXXFile;
class Context;
class Scope;
class MacroFuncScope;
class Expr;
class Stmt;
class Type;
class Struct;
class Union;
class Enum;
class Decl;
class TopLevel;
class Directive;
class TypeAlias;
class Attribute;
class QualNameExpr;
} // namespace namecxx

#endif
