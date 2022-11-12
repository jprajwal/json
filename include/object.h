#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "json.h"
#include "string.h"

#include <iostream>
#include <map>

namespace json {
class Json;
class Object;
using PairOfStrings = std::pair<String, String>;
using PairOfStringAndJson = std::pair<String, Json>;

class Object : public std::map<String, Json> {
public:
  Object() = default;
  Object(std::initializer_list<PairOfStringAndJson> &&);
  friend std::ostream &operator<<(std::ostream &out, Object &object);
};
} // namespace json

#endif