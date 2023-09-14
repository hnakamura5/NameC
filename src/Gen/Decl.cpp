#include "internal/Gen.h"

using namespace namec;

void RawDecl::emit_impl(std::stringstream &SS) { SS << get_val(); }

void VarDecl::emit_impl(std::stringstream &SS) {
  if (is_const()) {
    SS << "const ";
  }
  if (is_extern()) {
    SS << "extern ";
  }
  if (is_static()) {
    SS << "static ";
  }
  if (is_volatile()) {
    SS << "volatile ";
  }
  SS << get_type();
  SS << " ";
  if (is_restrict()) {
    SS << "restrict ";
  }
  SS << get_name();
  if (get_init()) {
    SS << " = ";
    get_init()->emit(SS);
  }
}

void ArrayVarDecl::emit_impl(std::stringstream &SS) {
  SS << get_type();
  SS << " ";
  SS << get_name();
  SS << "[";
  for (auto S : get_size()) {
    S->emit(SS);
    SS << "][";
  }
  SS << "]";
  if (get_init()) {
    SS << " = ";
    get_init()->emit(SS);
  }
}

void TypedefDecl::emit_impl(std::stringstream &SS) {
  auto *T = get_type_alias()->get_type();
  SS << "typedef ";
  if (auto *FT = cast<Function>(T)) {
    FT->get_ret_type()->emit(SS);
    SS << " ";
    SS << get_type_alias()->get_name();
    SS << "(";
    for (auto P : FT->get_params()) {
      P->emit(SS);
      SS << ", ";
    }
    SS << ")";
  } else {
    T->emit(SS);
    SS << " ";
    SS << get_type_alias()->get_name();
  }
}

void FuncDecl::emit_impl(std::stringstream &SS) {
  SS << get_ret_type();
  SS << " ";
  SS << get_name();
  SS << "(";
  for (auto P : get_params()) {
    P->emit(SS);
    SS << ", ";
  }
  SS << ")";
}

void StructDecl::emit_impl(std::stringstream &SS) { S->emit(SS); }
void UnionDecl::emit_impl(std::stringstream &SS) { U->emit(SS); }
void EnumDecl::emit_impl(std::stringstream &SS) { E->emit(SS); }
