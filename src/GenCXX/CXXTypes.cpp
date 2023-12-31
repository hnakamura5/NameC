#include "internal/GenCXX.h"

using namespace namecxx;

void RawType::emit_impl(std::ostream &SS) { SS << get_val(); }

void Void::emit_impl(std::ostream &SS) { SS << "void"; }

void Named::emit_impl(std::ostream &SS) { SS << D->get_name_str(); }

void TypeAlias::emit_impl(std::ostream &SS) { SS << get_name_str(); }

void Pointer::emit_impl(std::ostream &SS) { SS << get_elm_type() << "*"; }

void Reference::emit_impl(std::ostream &SS) { SS << get_elm_type() << "&"; }

void Array::emit_impl(std::ostream &SS) {
  SS << get_elm_type() << "[" << join(size(), "][") << "]";
}

void Function::emit_impl(std::ostream &SS) {
  SS << get_ret_type() << "(" << join(params()) << ")";
}

ClassTopLevel *ClassOrUnion::add_visibility_scope(AccessSpec AS) {
  auto *T = C.add_class_top_level(this);
  VisibilityScopes.push_back({AS, T});
  return T;
}

void ClassOrUnion::emit_impl(std::ostream &SS) {
  SS << "{";
  for (auto &[Visibility, Scope] : visibility_scopes()) {
    SS << Visibility.get_name() << ":\n";
    SS << Scope;
  }
  SS << "}";
}

void Class::emit_impl(std::ostream &SS) {
  if (is_struct()) {
    SS << "struct ";
  } else {
    SS << "class ";
  }
  SS << get_name_str();
  if (is_final()) {
    SS << " final";
  }
  if (base_size() > 0) {
    SS << " : ";
    SS << join_map(bases(), ",", [](auto Base) {
      auto &[Access, BaseTy] = *Base;
      return Access.get_name() + " " + BaseTy->to_string();
    });
  }
  ClassOrUnion::emit_impl(SS);
}

void Union::emit_impl(std::ostream &SS) {
  SS << "union " << get_name_str();
  ClassOrUnion::emit_impl(SS);
}

void Enum::emit_impl(std::ostream &SS) {
  SS << "enum " << get_name_str() << "{";
  for (auto &M : members()) {
    SS << M.first.to_string() << "=" << M.second << ",";
  }
  SS << "}";
}

void EnumClass::emit_impl(std::ostream &SS) {
  SS << "enum ";
  if (is_struct()) {
    SS << "struct ";
  } else {
    SS << "class ";
  }
  SS << get_name_str() << "{";
  for (auto &M : members()) {
    SS << M.first.to_string() << "=" << M.second << ",";
  }
  SS << "}";
}

void Instantiation::emit_impl(std::ostream &SS) {
  SS << TD->get_name_str() << "<" << join(args()) << ">";
}

void PackedType::emit_impl(std::ostream &SS) { SS << get_type() << "..."; }

void TypeNameType::emit_impl(std::ostream &SS) {
  if (is_class()) {
    SS << "class";
  } else {
    SS << "typename";
  }
}
