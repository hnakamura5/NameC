#include "internal/Gen.h"

using namespace namec;

void RawDecl::emit_impl(std::ostream &SS) { SS << get_val(); }

void VarDecl::emit_impl(std::ostream &SS) {
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
  auto Name = get_name();
  if (Name.size() > 0) {
    SS << " ";
  }
  if (is_restrict()) {
    SS << "restrict ";
  }
  SS << Name;
  if (get_init()) {
    SS << "=" << get_init();
  }
}

void ArrayVarDecl::emit_impl(std::ostream &SS) {
  SS << get_type() << " " << get_name();
  SS << "[" << join(sizes(), "][") << "]";
  if (get_init()) {
    SS << "=" << get_init();
  }
}

std::string TypedefDecl::get_name() { return TA->get_name(); }

void TypedefDecl::emit_impl(std::ostream &SS) {
  auto *T = get_type_alias()->get_type();
  SS << "typedef ";
  if (auto *FT = cast<Function>(T)) {
    SS << FT->get_ret_type() << " " << get_type_alias()->get_name() << "(";
    for (auto P : FT->get_params()) {
      P->emit(SS);
      SS << ",";
    }
    SS << ")";
  } else {
    T->emit(SS);
    SS << T << " " << get_type_alias()->get_name();
  }
}

FuncScope *FuncDecl::get_or_add_body() {
  if (!Body) {
    Body = C.add_scope();
  }
  return Body;
}

void FuncDecl::emit_impl_impl(std::ostream &SS, bool IsForward,
                              bool IsSplitDefinition) {
  SS << get_ret_type() << " " << get_name() << "(" << join(params());
  if (is_vararg()) {
    SS << ",...";
  }
  SS << ")";
  if (!IsForward) {
    SS << "{" << Body << "}";
  } else {
    // Forward declaration
    SS << ";";
  }
}

void FuncDecl::emit_impl(std::ostream &SS) {
  return emit_impl_impl(SS, !Body, false);
}

void FuncSplitForwardDecl::emit_impl(std::ostream &SS) {
  FD->emit_impl_impl(SS, true, true);
}

void FuncSplitDecl::emit_impl(std::ostream &SS) {
  emit_impl_impl(SS, false, true);
}

std::string StructDecl::get_name() { return S->get_name(); }
void StructDecl::emit_impl(std::ostream &SS) {
  if (IsForward) {
    SS << "struct " << get_name();
  } else {
    S->emit(SS);
  }
}

std::string UnionDecl::get_name() { return U->get_name(); }
void UnionDecl::emit_impl(std::ostream &SS) {
  if (IsForward) {
    SS << "union " << get_name();
  } else {
    U->emit(SS);
  }
}

std::string EnumDecl::get_name() { return E->get_name(); }
void EnumDecl::emit_impl(std::ostream &SS) {
  if (IsForward) {
    SS << "enum " << get_name();
  } else {
    E->emit(SS);
  }
}
