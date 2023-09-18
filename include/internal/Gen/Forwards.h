#ifndef NAMEC_GEN_FORWARDS_H
#define NAMEC_GEN_FORWARDS_H

#include <iosfwd>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "internal/Util/IteratorRange.h"
#include "internal/Util/VUIterator.h"

namespace namec {
class CFile;
class Context;
class Scope;
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
} // namespace namec

#endif // NAMEC_GEN_FORWARDS_H
