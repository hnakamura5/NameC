#include "internal/GenCXX.h"

using namespace namecxx;

std::vector<std::string>
Context::qual_name_concat(QualName Top, std::vector<std::string> Other) {
  std::vector<std::string> Names;
  for (auto Name : Top.get_names()) {
    Names.push_back(Name);
  }
  for (auto Name : Other) {
    Names.push_back(Name);
  }
  return Names;
}

std::vector<std::string>
Context::qual_name_concat(Decl *Top, std::vector<std::string> Other) {
  std::vector<std::string> Names;
  return qual_name_concat(Top->get_name_str(), Other);
}

FuncScope *Context::add_func_scope() {
  auto *Ret = new FuncScope(*this);
  FuncScopes.push_back(std::unique_ptr<FuncScope>(Ret));
  return Ret;
}

TopLevel *Context::add_top_level() {
  auto *Ret = new TopLevel(*this);
  TopLevels.push_back(std::unique_ptr<TopLevel>(Ret));
  return Ret;
}

MacroFuncScope *Context::add_macro_func_scope() {
  auto *Ret = new MacroFuncScope(*this);
  MacroFuncScopes.push_back(std::unique_ptr<MacroFuncScope>(Ret));
  return Ret;
}

ClassTopLevel *Context::add_class_top_level(ClassOrUnion *Cls) {
  auto *Ret = new ClassTopLevel(*this, Cls);
  ClassTopLevels.push_back(std::unique_ptr<ClassTopLevel>(Ret));
  return Ret;
}
