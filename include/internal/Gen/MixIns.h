#ifndef NAMEC_GEN_MIXINS_H
#define NAMEC_GEN_MIXINS_H

#include "internal/Gen/Forwards.h"

namespace namec {

class CommentMixin {
  std::string CommentBefore;
  std::string CommentAfter;

public:
  void set_comment_before(std::string CommentBefore) {
    this->CommentBefore = CommentBefore;
  }
  std::string get_comment_before() { return CommentBefore; }
  void set_comment_after(std::string CommentAfter) {
    this->CommentAfter = CommentAfter;
  }
  std::string get_comment_after() { return CommentAfter; }
};

} // namespace namec

#endif // NAMEC_GEN_MIXINS_H
