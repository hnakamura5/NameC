#include "internal/Gen.h"

using namespace namec;

void RawType::emit_impl(std::ostream &SS) { SS << get_val(); }

void Void::emit_impl(std::ostream &SS) { SS << "void"; }

void Named::emit_impl(std::ostream &SS) {
  if (auto *S = cast<Struct>(D)) {
    SS << "struct " << S->get_name();
  } else if (auto *U = cast<Union>(D)) {
    SS << "union " << U->get_name();
  } else if (auto *E = cast<Enum>(D)) {
    SS << "enum " << E->get_name();
  } else {
    D->emit(SS);
  }
}

void TypeAlias::emit_impl(std::ostream &SS) { SS << get_name(); }

void Pointer::emit_impl(std::ostream &SS) {
  get_elm_type()->emit(SS);
  SS << "*";
}

void Array::emit_impl(std::ostream &SS) {
  get_elm_type()->emit(SS);
  SS << "[";
  for (auto S : get_size()) {
    S->emit(SS);
    SS << "][";
  }
  SS << "]";
}

void Function::emit_impl(std::ostream &SS) {
  get_ret_type()->emit(SS);
  SS << "(";
  for (auto P : get_params()) {
    P->emit(SS);
    SS << ", ";
  }
  SS << ")";
}

void Struct::emit_impl(std::ostream &SS) {
  SS << "struct ";
  SS << get_name();
  SS << " {";
  for (auto &M : members()) {
    M.emit(SS);
    SS << "; ";
  }
  SS << "}";
}

void Union::emit_impl(std::ostream &SS) {
  SS << "union ";
  SS << get_name();
  SS << " {";
  for (auto &M : members()) {
    M.emit(SS);
    SS << "; ";
  }
  SS << "}";
}

void Enum::emit_impl(std::ostream &SS) {
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
