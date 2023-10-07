#include "internal/GenCXX.h"

using namespace namecxx;

void RawDecl::emit_impl(std::ostream &SS) { SS << get_val(); }

static void emit_type_qual(std::ostream &SS, VarDecl *D) {
  if (D->is_const()) {
    SS << "const ";
  }
  if (D->is_extern()) {
    SS << "extern ";
  }
  if (D->is_static()) {
    SS << "static ";
  }
  if (D->is_volatile()) {
    SS << "volatile ";
  }
  if (D->is_restrict()) {
    SS << "restrict ";
  }
  if (D->is_inline()) {
    SS << "inline ";
  }
}

void VarDecl::emit_impl(std::ostream &SS) {
  emit_type_qual(SS, this);
  SS << get_type();
  SS << " ";
  if (is_restrict()) {
    SS << "restrict ";
  }
  SS << get_name_str();
  if (get_init()) {
    SS << "=";
    SS << get_init();
  }
}

void ArrayVarDecl::emit_impl(std::ostream &SS) {
  emit_type_qual(SS, this);
  SS << get_type() << " " << get_name_str();
  SS << "[" << join(sizes(), "][") << "]";
  if (get_init()) {
    SS << "=";
    SS << get_init();
  }
}

void VarInitializeDecl::emit_impl(std::ostream &SS) {
  emit_type_qual(SS, this);
  SS << get_type();
  if (is_list_init()) {
    SS << "{";
  } else {
    SS << "(";
  }
  SS << get_init();
}

QualName TypedefDecl::get_name() { return TA->get_name_str(); }
void TypedefDecl::emit_impl(std::ostream &SS) {
  auto *T = get_type_alias()->get_type();
  SS << "typedef ";
  if (auto *FT = cast<Function>(T)) {
    SS << FT->get_ret_type();
    SS << " ";
    SS << get_type_alias()->get_name_str();
    SS << "(";
    for (auto P : FT->get_params()) {
      SS << P;
      SS << ", ";
    }
    SS << ")";
  } else {
    SS << T;
    SS << " ";
    SS << get_type_alias()->get_name_str();
  }
}

QualName UsingDecl::get_name() { return TA->get_name_str(); }
void UsingDecl::emit_impl(std::ostream &SS) {
  SS << "using ";
  SS << get_type_alias()->get_name_str();
  SS << "=";
  SS << get_type_alias()->get_type();
}

FuncScope *FuncDecl::get_or_add_body() {
  if (!Body) {
    Body = C.add_func_scope();
  }
  return Body;
}

static void emit_func_qual(std::ostream &SS, FuncDecl *D) {
  if (D->is_extern()) {
    SS << "extern ";
  }
  if (D->is_static()) {
    SS << "static ";
  }
  if (D->is_constexpr()) {
    SS << "constexpr ";
  }
  if (D->is_inline()) {
    SS << "inline ";
  }
}

void FuncDecl::emit_impl(std::ostream &SS) {
  emit_func_qual(SS, this);
  SS << get_ret_type();
  SS << " ";
  SS << get_name_str();
  SS << "(";
  for (auto I = 0; I < Params.size(); ++I) {
    SS << Params[I];
    if (I != Params.size() - 1) {
      SS << ",";
    }
  }
  if (is_vararg()) {
    SS << ",...";
  }
  SS << ")";
  if (Body) {
    SS << "{";
    SS << Body;
    SS << "}";
  } else {
    // Forward declaration
    SS << ";";
  }
}

QualName ClassDecl::get_name() { return C->get_name_str(); }
void ClassDecl::emit_impl(std::ostream &SS) {
  if (IsForward) {
    if (C->is_struct()) {
      SS << "struct " << get_name_str();
    } else {
      SS << "class " << get_name_str();
    }
  } else {
    SS << C;
  }
}

QualName UnionDecl::get_name() { return U->get_name_str(); }
void UnionDecl::emit_impl(std::ostream &SS) {
  if (IsForward) {
    SS << "union " << get_name_str();
  } else {
    SS << U;
  }
}

static void emit_method_qual(std::ostream &SS, MethodDecl *D) {
  if (D->is_override()) {
    SS << "override ";
  }
  if (D->is_const()) {
    SS << "const ";
  }
  if (D->is_abstract()) {
    SS << "=0;";
    return;
  }
  if (D->is_default()) {
    SS << "=default;";
    return;
  }
  if (D->is_delete()) {
    SS << "=delete;";
    return;
  }
}

void MethodDecl::emit_impl(std::ostream &SS) {
  emit_func_qual(SS, this);
  if (is_virtual()) {
    SS << "virtual ";
  }
  if (get_ret_type()) {
    // Allowing null return type for ctor/dtor
    SS << get_ret_type();
  }
  SS << " ";
  SS << get_name_str();
  SS << "(" << join(params()) << ")";
  if (is_vararg()) {
    SS << ",...";
  }
  SS << ")";
  emit_method_qual(SS, this);
  if (Body) {
    SS << "{" << Body << "}";
  } else {
    // Forward declaration
    SS << ";";
  }
}

CtorDecl::CtorDecl(Context &C, ClassOrUnion *Parent,
                   std::vector<VarDecl *> Params, bool IsVarArg)
    : MethodDecl(C, Parent, Name, nullptr, Params, IsVarArg) {}

void CtorDecl::emit_impl(std::ostream &SS) {
  emit_func_qual(SS, this);
  SS << Parent->get_name().last();
  SS << "(";
  for (auto I = 0; I < Params.size(); ++I) {
    SS << Params[I];
    if (I != Params.size() - 1) {
      SS << ",";
    }
  }
  if (is_vararg()) {
    SS << ",...";
  }
  SS << ")";
  emit_method_qual(SS, this);
  if (Inits.size() > 0) {
    SS << ":" << join_map(inits(), ",", [](auto &P) {
      return P->first.to_string() + "(" + P->second->to_string() + ")";
    });
  }
  if (Body) {
    SS << "{" << Body << "}";
  } else {
    // Forward declaration
    SS << ";";
  }
}

QualName EnumDecl::get_name() { return E->get_name_str(); }
void EnumDecl::emit_impl(std::ostream &SS) {
  if (IsForward) {
    SS << "enum " << get_name_str();
  } else {
    SS << E;
  }
}

void TemplateDecl::emit_impl(std::ostream &SS) {
  SS << "template<";
  for (auto I = 0; I < Params.size(); ++I) {
    SS << Params[I];
    if (I != Params.size() - 1) {
      SS << ",";
    }
  }
  SS << "> ";
  SS << D;
}
