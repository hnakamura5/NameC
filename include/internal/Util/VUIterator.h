#ifndef NAMEC_UTIL_VU_ITERATOR_H
#define NAMEC_UTIL_VU_ITERATOR_H

#include <iterator>
#include <memory>
#include <vector>

namespace namec_util {
template <typename T>
class VUIterator : public std::iterator<std::forward_iterator_tag, T> {
  std::vector<std::unique_ptr<T>> *Vec;
  size_t Index;

public:
  VUIterator() = default;
  VUIterator(std::vector<std::unique_ptr<T>> &Vec, size_t Index)
      : Vec(&Vec), Index(Index) {}

  VUIterator &operator++() {
    ++Index;
    return *this;
  }

  VUIterator operator++(int) {
    VUIterator Tmp = *this;
    ++(*this);
    return Tmp;
  }

  bool operator==(const VUIterator &RHS) const {
    return Index == RHS.Index && Vec == RHS.Vec;
  }

  bool operator!=(const VUIterator &RHS) const { return !(*this == RHS); }

  T &operator*() const { return *Vec->at(Index).get(); }

  T *operator->() const { return Vec->at(Index).get(); }
};
} // namespace namec_util

#endif // NAMEC_UTIL_VU_ITERATOR_H
