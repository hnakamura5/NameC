#ifndef NAMEC_GENCXX_COMMON_H
#define NAMEC_GENCXX_COMMON_H

#include "internal/GenCXX/CXXForwards.h"

namespace namecxx {

class Emit {
  std::string CommentBefore;
  std::string CommentAfter;

protected:
  virtual void emit_impl(std::ostream &SS) = 0;

public:
  void set_comment_before(std::string CommentBefore) {
    this->CommentBefore = CommentBefore;
  }
  std::string get_comment_before() { return CommentBefore; }
  void set_comment_after(std::string CommentAfter) {
    this->CommentAfter = CommentAfter;
  }
  std::string get_comment_after() { return CommentAfter; }

  void emit(std::ostream &SS) {
    if (!CommentBefore.empty()) {
      SS << "/* " << CommentBefore << " */";
    }
    emit_impl(SS);
    if (!CommentAfter.empty()) {
      SS << "/* " << CommentAfter << " */";
    }
  }

  std::string to_string() {
    std::stringstream SS;
    emit(SS);
    return SS.str();
  }
};

class QualName : public Emit {
  std::vector<std::string> Names;

public:
  using iterator = decltype(Names)::iterator;
  QualName(std::vector<std::string> Names) : Names(Names) {}
  std::vector<std::string> get_names() { return Names; }
  IteratorRange<iterator> names() {
    return IteratorRange<iterator>(Names.begin(), Names.end());
  }

protected:
  void emit_impl(std::ostream &SS) override;
};

} // namespace namecxx

#endif // NAMEC_GENCXX_COMMON_H
