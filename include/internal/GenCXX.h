#ifndef NAMEC_GENCXX_H
#define NAMEC_GENCXX_H

#include "internal/Gen.h"
#include "internal/GenCXX/CXXContext.h"
#include "internal/GenCXX/CXXDecl.h"
#include "internal/GenCXX/CXXExprs.h"
#include "internal/GenCXX/CXXFile.h"
#include "internal/GenCXX/CXXForwards.h"
#include "internal/GenCXX/CXXScope.h"
#include "internal/GenCXX/CXXStmts.h"
#include "internal/GenCXX/CXXTypes.h"

namespace namecxx {
// standard way to cast a pointer
template <typename T, typename S> T *cast(S *Ptr) {
  return dynamic_cast<T *>(Ptr);
}

} // namespace namecxx

#endif // NAMEC_GENCXX_H
