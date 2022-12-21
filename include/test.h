#ifndef JSON_TEST_H
#define JSON_TEST_H
#include <algorithm>
#include <bits/c++config.h>
#include <iostream>
#include <ostream>

namespace json {

class Log : public std::ostream {

public:
  template <typename T> Log &operator<<(const T &item) {
#ifndef NDEBUG
    std::cerr << item;
#endif
    return *this;
  }

  Log &operator<<(std::ostream &(*f)(std::ostream &)) {
#ifndef NDEBUG
    std::cerr << f;
#endif
    return *this;
  }
};

Log log{};

template <typename T> void print_list(const T &list) {
  log << '[';
  std::size_t index = 0;
  for (const auto &item : list) {
    if (index > 0) {
      log << ", ";
    }
    log << item;
    ++index;
  }
  log << ']' << std::endl;
}

template <typename T1, typename T2>
bool is_same_list(const T1 &list1, const T2 &list2) {
  return std::equal(list1.begin(), list1.end(), list2.begin());
}

} // namespace json
#endif
