#ifndef NAMEC_GEN_H
#define NAMEC_GEN_H

#include "internal/Gen/Context.h"
#include "internal/Gen/Decl.h"
#include "internal/Gen/Directive.h"
#include "internal/Gen/Emit.h"
#include "internal/Gen/Exprs.h"
#include "internal/Gen/File.h"
#include "internal/Gen/Forwards.h"
#include "internal/Gen/MixIns.h"
#include "internal/Gen/Scope.h"
#include "internal/Gen/Stmts.h"
#include "internal/Gen/Types.h"

namespace namec {
// standard way to cast a pointer
template <typename T, typename S> T *cast(S *Ptr) {
  return dynamic_cast<T *>(Ptr);
}

} // namespace namec

#endif // NAMEC_GEN_H
