#ifndef NAMEC_GEN_FILE_H
#define NAMEC_GEN_FILE_H

#include <memory>
#include <string>
#include <vector>

namespace namec {
struct TopLevel;

class File {

  std::vector<std::unique_ptr<TopLevel>> TopLevels;

public:
  void include(std::string Path);
  void include_sys(std::string Path);
  void def_macro_flag(std::string Name);
  void def_macro_value(std::string Name, std::string Value);
  // TODO: void def_macro_func
  void def_macro_undef(std::string Name);

  void emit(std::stringstream &SS);
};

} // namespace namec

#endif
