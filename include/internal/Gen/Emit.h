#ifndef NAMEC_GEN_EMIT_H
#define NAMEC_GEN_EMIT_H

#include "internal/Gen/Forwards.h"

namespace namec {

class Emit {
  std::string CommentBefore;
  std::string CommentAfter;

protected:
  virtual void emit_impl(std::stringstream &SS) = 0;

public:
  void set_comment_before(std::string CommentBefore) {
    this->CommentBefore = CommentBefore;
  }
  std::string get_comment_before() { return CommentBefore; }
  void set_comment_after(std::string CommentAfter) {
    this->CommentAfter = CommentAfter;
  }
  std::string get_comment_after() { return CommentAfter; }

  void emit(std::stringstream &SS) {
    if (!CommentBefore.empty()) {
      SS << "/* " << CommentBefore << " */";
    }
    emit_impl(SS);
    if (!CommentAfter.empty()) {
      SS << "/* " << CommentAfter << " */";
    }
  }
};

} // namespace namec

#endif // NAMEC_GEN_EMIT_H
