#include "internal/GenCXX.h"

using namespace namecxx;

std::string QualName::to_string() {
  std::stringstream SS;
  SS << join(names(), "::");
  return SS.str();
}
