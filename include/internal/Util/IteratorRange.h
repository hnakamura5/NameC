#ifndef NAMEC_UTIL_ITERATOR_RANGE_H
#define NAMEC_UTIL_ITERATOR_RANGE_H

#include <string>

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

template <typename OS, typename IterT, typename MapT>
OS &join_out(OS &SS, IteratorRange<IterT> Range, std::string Sep, MapT Map) {
  for (auto I = Range.begin(), E = Range.end(); I != E; ++I) {
    if (I != Range.begin()) {
      SS << Sep;
    }
    SS << Map(I);
  }
  return SS;
}

template <typename IterT>
std::string join(IteratorRange<IterT> Range, std::string Sep = ",") {
  std::stringstream SS;
  join_out(SS, Range, Sep, [](IterT X) { return *X; });
  return SS.str();
}

template <typename IterT, typename MapT>
std::string join_map(IteratorRange<IterT> Range, std::string Sep, MapT Map) {
  std::stringstream SS;
  join_out(SS, Range, Sep, Map);
  return SS.str();
}

} // namespace namec_util

#endif // NAMEC_UTIL_ITERATOR_RANGE_H
