#include "internal/Gen.h"

using namespace namec;

void RawType::emit_impl(std::ostream &SS) { SS << get_val(); }

void Void::emit_impl(std::ostream &SS) { SS << "void"; }

void Named::emit_impl(std::ostream &SS) {
  if (auto *S = cast<StructDecl>(D)) {
    SS << "struct " << S->get_name();
  } else if (auto *U = cast<UnionDecl>(D)) {
    SS << "union " << U->get_name();
  } else if (auto *E = cast<EnumDecl>(D)) {
    SS << "enum " << E->get_name();
  } else {
    D->emit(SS);
  }
}

void TypeAlias::emit_impl(std::ostream &SS) { SS << get_name(); }

void Pointer::emit_impl(std::ostream &SS) { SS << get_elm_type() << "*"; }

void Array::emit_impl(std::ostream &SS) {
  SS << get_elm_type() << "[" << join(sizes(), "][") << "]";
}

void Function::emit_impl(std::ostream &SS) {
  get_ret_type()->emit(SS);
  SS << "(" << join(params(), ",") << ")";
}

void Struct::emit_impl(std::ostream &SS) {
  SS << "struct " << get_name() << "{";
  for (auto &M : members()) {
    M.emit(SS);
    SS << ";";
  }
  SS << "}";
}

void Union::emit_impl(std::ostream &SS) {
  SS << "union " << get_name() << "{";
  for (auto &M : members()) {
    M.emit(SS);
    SS << ";";
  }
  SS << "}";
}

void Enum::emit_impl(std::ostream &SS) {
  SS << "enum " << get_name() << "{";
  for (auto &M : members()) {
    SS << M.first << "=" << M.second << ",";
  }
  SS << "}";
}
