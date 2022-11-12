#include "object.h"

namespace json {

Object::Object(std::initializer_list<PairOfStringAndJson> &&pairs)
    : std::map<String, Json>{} {
  for (auto &&pair : pairs) {
    this->emplace(std::move(pair));
  }
}

std::ostream &operator<<(std::ostream &out, Object &object) {
  out << "{";
  auto count{object.size()};
  for (auto const &pair : object) {
    out << pair.first << ":";
    out << pair.second;
    if (count > 1) {
      out << ",";
    }
    --count;
  }
  out << "}";
  return out;
}
} // namespace json