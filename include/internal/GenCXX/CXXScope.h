#ifndef NAMEC_GENCXX_SCOPE_H
#define NAMEC_GENCXX_SCOPE_H

#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {

class CXXScope : public namec::Scope {

public:
  CXXScope() = default;
  ~CXXScope() = default;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_SCOPE_H
