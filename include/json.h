#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "null.h"

#include <iostream>
#include <map>
#include <stdexcept>
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
  Json(const char *str) : mType{Type::String}, mStr{str} {}
  Json(const std::string &str) : mType{Type::String}, mStr{str} {}
  Json(std::string &&str) : mType{Type::String}, mStr{std::move(str)} {}

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

  Type type() { return mType; }

  bool operator==(const Json &other) {
    if (mType != other.mType) {
      return false;
    }
    switch (mType) {
    case Type::String:
      return mStr == other.mStr;
    default:
      return false;
    }
  }

  using ConstStringIterator = std::string::const_iterator;

  ConstStringIterator stringCBegin() {
    if (mType != Type::String) {
      throw std::runtime_error{
          "stringBegin() is supported only for json strings"};
    }
    return mStr.cbegin();
  }

  ConstStringIterator stringCEnd() {
    if (mType != Type::String) {
      throw std::runtime_error{
          "stringEnd() is supported only for json strings"};
    }
    return mStr.cend();
  }

  friend std::ostream &operator<<(std::ostream &out, const Json &js) {
    switch (js.mType) {
    case Type::String:
      out << js.mStr;
      break;
    case Type::Null:
      out << "null";
      break;
    default:
      out << "Unimplemented";
    }
    return out;
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
