#include "internal/Gen.h"

using namespace namec;

void RawType::emit_impl(std::stringstream &SS) { SS << get_val(); }

void Void::emit_impl(std::stringstream &SS) { SS << "void"; }

void Named::emit_impl(std::stringstream &SS) {
  get_decl()->emit(SS);
  SS << " ";
}

void TypeAlias::emit_impl(std::stringstream &SS) { SS << get_name(); }

void Pointer::emit_impl(std::stringstream &SS) {
  get_elm_type()->emit(SS);
  SS << "*";
}

void Array::emit_impl(std::stringstream &SS) {
  get_elm_type()->emit(SS);
  SS << "[";
  for (auto S : get_size()) {
    S->emit(SS);
    SS << "][";
  }
  SS << "]";
}

void Function::emit_impl(std::stringstream &SS) {
  get_ret_type()->emit(SS);
  SS << "(";
  for (auto P : get_params()) {
    P->emit(SS);
    SS << ", ";
  }
  SS << ")";
}

void Struct::emit_impl(std::stringstream &SS) {
  SS << "struct ";
  SS << get_name();
  SS << " {";
  for (auto &M : members()) {
    M.emit(SS);
    SS << "; ";
  }
  SS << "}";
}

void Union::emit_impl(std::stringstream &SS) {
  SS << "union ";
  SS << get_name();
  SS << " {";
  for (auto &M : members()) {
    M.emit(SS);
    SS << "; ";
  }
  SS << "}";
}

void Enum::emit_impl(std::stringstream &SS) {
  SS << "enum ";
  SS << get_name();
  SS << " {";
  for (auto &M : members()) {
    SS << M.first << " = ";
    M.second->emit(SS);
    SS << ", ";
  }
  SS << "}";
}
