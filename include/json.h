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

template <typename CharT = char> class JsonT {
public:
  using string_t = std::basic_string<CharT>;
  using s_const_iterator = typename string_t::const_iterator;

  JsonT() : mNull{} {}
  JsonT(const char *str) : mType{Type::String}, mStr{str} {}
  JsonT(const string_t &str) : mType{Type::String}, mStr{str} {}
  JsonT(string_t &&str) : mType{Type::String}, mStr{std::move(str)} {}

  ~JsonT() {
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

  bool operator==(const JsonT &other) {
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

  s_const_iterator s_cbegin() {
    if (mType != Type::String) {
      throw std::runtime_error{"s_cbegin() is supported only for json strings"};
    }
    return mStr.cbegin();
  }

  s_const_iterator s_cend() {
    if (mType != Type::String) {
      throw std::runtime_error{"s_cend() is supported only for json strings"};
    }
    return mStr.cend();
  }

  friend std::ostream &operator<<(std::ostream &out, const JsonT &js) {
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
    string_t mStr;
  };
};

// Typedefs
typedef JsonT<char> Json;
typedef JsonT<char16_t> WJson;
typedef JsonT<char32_t> QJson;

} // namespace json

#endif
