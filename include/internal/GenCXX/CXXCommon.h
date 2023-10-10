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
  virtual ~Emit() {}
  void set_comment_before(std::string CommentBefore) {
    this->CommentBefore = CommentBefore;
  }
  std::string get_comment_before() { return CommentBefore; }
  void set_comment_after(std::string CommentAfter) {
    this->CommentAfter = CommentAfter;
  }
  std::string get_comment_after() { return CommentAfter; }

  virtual void emit(std::ostream &SS) {
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

class AttrEmit : public Emit {
  std::vector<Attribute *> Attrs;

public:
  using attr_iterator = decltype(Attrs)::iterator;
  void add_attr(Attribute *Attr) { Attrs.push_back(Attr); }
  IteratorRange<attr_iterator> attrs() {
    return IteratorRange<attr_iterator>(Attrs.begin(), Attrs.end());
  }
  void emit(std::ostream &SS) override;
};

class TypeOrExpr : public Emit {};

class QualName {
  std::vector<std::string> Names;

public:
  using iterator = decltype(Names)::iterator;
  QualName(std::vector<std::string> Names) : Names(Names) {}
  QualName(std::string Name) { Names.push_back(Name); }
  QualName(const char *Name) { Names.push_back(Name); }
  std::vector<std::string> get_names() { return Names; }
  IteratorRange<iterator> names() {
    return IteratorRange<iterator>(Names.begin(), Names.end());
  }
  std::string last() { return Names.back(); }
  std::string to_string();
};

template <typename OS> OS &operator<<(OS &SS, Emit *E) {
  E->emit(SS);
  return SS;
}

} // namespace namecxx

#endif // NAMEC_GENCXX_COMMON_H
