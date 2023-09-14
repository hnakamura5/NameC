#include "internal/Gen.h"

using namespace namec;

void RawStmt::emit_impl(std::stringstream &SS) { SS << get_val(); }

void DeclStmt::emit_impl(std::stringstream &SS) {
  get_decl()->emit(SS);
  SS << ";";
}

void If::emit_impl(std::stringstream &SS) {
  SS << "if (";
  get_cond()->emit(SS);
  SS << ") {";
  get_then()->emit(SS);
  SS << " }";
  if (get_else()) {
    SS << " else {";
    get_else()->emit(SS);
    SS << " }";
  }
}

void While::emit_impl(std::stringstream &SS) {
  SS << "while (";
  get_cond()->emit(SS);
  SS << ") {";
  get_body()->emit(SS);
  SS << " }";
}

void For::emit_impl(std::stringstream &SS) {
  SS << "for (";
  get_init()->emit(SS);
  SS << "; ";
  get_cond()->emit(SS);
  SS << "; ";
  get_step()->emit(SS);
  SS << ") {";
  get_body()->emit(SS);
  SS << " }";
}

void Do::emit_impl(std::stringstream &SS) {
  SS << "do {";
  get_body()->emit(SS);
  SS << " } while (";
  get_cond()->emit(SS);
  SS << ")";
}

void Block::emit_impl(std::stringstream &SS) {
  SS << "{\n";
  get_scope()->emit(SS);
  SS << "}";
}

void ExprStmt::emit_impl(std::stringstream &SS) {
  get_expr()->emit(SS);
  SS << ";";
}

void Return::emit_impl(std::stringstream &SS) {
  SS << "return";
  if (get_expr()) {
    SS << " ";
    get_expr()->emit(SS);
  }
  SS << ";";
}

void Break::emit_impl(std::stringstream &SS) { SS << "break;"; }

void Continue::emit_impl(std::stringstream &SS) { SS << "continue;"; }

void Label::emit_impl(std::stringstream &SS) {
  SS << get_name() << ": ";
  if (get_stmt()) {
    get_stmt()->emit(SS);
  } else {
    SS << ";";
  }
}

void Goto::emit_impl(std::stringstream &SS) {
  SS << "goto " << get_name() << ";";
}

void Switch::emit_impl(std::stringstream &SS) {
  SS << "switch (";
  get_cond()->emit(SS);
  SS << ") {";
  get_body()->emit(SS);
  SS << "}";
}
