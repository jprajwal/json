#ifndef JSON_NULL_H
#define JSON_NULL_H

#include "json/test.h"
#include <iostream>

namespace json {

class Null {
public:
  Null() = default;
  ~Null() = default;
  friend std::ostream &operator<<(std::ostream &out, const Null &) {
    out << "null";
    return out;
  }
};

} // namespace json
#endif
