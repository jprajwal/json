#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "null.h"

#include <map>
#include <string>
#include <utility>

namespace json {

enum class Type {
  Null,
  String,
  Object,
};

class Json {

public:
  Json() : mNull{} {}
  Json(const char *str) : mStr{str} {}
  Json(const std::string &str) : mStr{str} {}
  Json(std::string &&str) : mStr{std::move(str)} {}
  ~Json() {
    switch (mType) {
    case Type::String:
      mStr.~basic_string();
      break;
    case Type::Null:
      break;
    case Type::Object:
      break;
    }
  }

private:
  Type mType{Type::Null};
  union {
    Null mNull{};
    std::string mStr;
    // std::map<std::string, Json> mMap;
  };
};
} // namespace json

#endif
