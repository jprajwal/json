#ifndef JSON_TEST_H
#define JSON_TEST_H
#include <iostream>

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
} // namespace json
#endif