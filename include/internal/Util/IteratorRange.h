#ifndef NAMEC_UTIL_ITERATOR_RANGE_H
#define NAMEC_UTIL_ITERATOR_RANGE_H

namespace namec_util {
template <typename IteratorT> class IteratorRange {
  IteratorT Begin;
  IteratorT End;

public:
  IteratorRange() = default;
  IteratorRange(const IteratorRange<IteratorT> &) = default;
  IteratorRange(IteratorRange<IteratorT> &&) = default;
  IteratorRange<IteratorT> &
  operator=(const IteratorRange<IteratorT> &) = default;
  IteratorRange<IteratorT> &operator=(IteratorRange<IteratorT> &&) = default;
  ~IteratorRange() = default;

  IteratorRange(IteratorT Begin, IteratorT End) : Begin(Begin), End(End) {}

  IteratorT begin() const { return Begin; }
  IteratorT end() const { return End; }
  bool empty() const { return Begin == End; }
};
} // namespace namec_util

#endif // NAMEC_UTIL_ITERATOR_RANGE_H
