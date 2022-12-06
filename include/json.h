#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "null.h"

#include <initializer_list>
#include <iostream>
#include <iterator>
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
  using string_t = std::string;
  using object_t = std::map<string_t, Json>;

  // defaults
  Json() : mNull{} {}

  // Json strings
  Json(const char *str) : mType{Type::String}, mStrPtr{new string_t{str}} {}
  Json(const string_t &str) : mType{Type::String}, mStrPtr{new string_t{str}} {}
  Json(string_t &&str)
      : mType{Type::String}, mStrPtr{new string_t{std::move(str)}} {}

  // Json Objects
  Json(std::initializer_list<std::pair<const string_t, Json>> pairs)
      : mType{Type::Object}, mObjectPtr{new object_t{pairs}} {}

  // Copy constructor
  Json(const Json &other) : mType{other.mType}, mNull{} {
    switch (other.mType) {
    case Type::String:
      mStrPtr = new string_t{*other.mStrPtr};
      break;
    case Type::Object:
      mObjectPtr = new object_t{*other.mObjectPtr};
      break;
    case Type::Null:
      mNull = other.mNull;
      break;
    }
  }

  // Move constructor
  Json(Json &&other) noexcept : mType{other.mType}, mNull{} {
    switch (other.mType) {
    case Type::String:
      mStrPtr = other.mStrPtr;
      other.mType = Type::Null;
      other.mNull = Null{};
      break;
    case Type::Object:
      mObjectPtr = other.mObjectPtr;
      other.mType = Type::Null;
      other.mNull = Null{};
      break;
    case Type::Null:
      mNull = other.mNull;
      break;
    }
  }

  // Assignments
  // Copy Assignment
  Json &operator=(const Json &other) {
    if (this == &other) {
      return *this;
    }
    deleteCurrent();
    mType = other.mType;

    switch (other.mType) {
    case Type::String:
      mStrPtr = new string_t{*other.mStrPtr};
      break;
    case Type::Object:
      mObjectPtr = new object_t{*other.mObjectPtr};
      break;
    case Type::Null:
      mNull = other.mNull;
      break;
    }
    return *this;
  }

  // Move Assignment
  Json &operator=(Json &&other) noexcept {
    deleteCurrent();
    mType = other.mType;

    switch (other.mType) {
    case Type::String:
      mStrPtr = other.mStrPtr;
      break;
    case Type::Object:
      mObjectPtr = other.mObjectPtr;
      break;
    case Type::Null:
      mNull = other.mNull;
      break;
    }

    other.mType = Type::Null;
    other.mNull = Null{};
    return *this;
  }

  ~Json() { deleteCurrent(); }

  Type type() const { return mType; }

  bool operator==(const Json &other) const {
    if (mType != other.mType) {
      return false;
    }
    switch (mType) {
    case Type::Null:
      return true;
    case Type::String:
      return *mStrPtr == *other.mStrPtr;
    case Type::Object:
      return *mObjectPtr == *other.mObjectPtr;
    default:
      return false;
    }
  }

  Json &operator[](const string_t &key) {
    assertObject();
    return (*mObjectPtr)[key];
  }

  typename object_t::iterator objFind(const string_t &key) {
    assertObject();
    return mObjectPtr->find(key);
  }

  typename object_t::const_iterator objFind(const string_t &key) const {
    assertObject();
    return mObjectPtr->find(key);
  }

  typename object_t::size_type objErase(const string_t &key) {
    assertObject();
    return mObjectPtr->erase(key);
  }

  friend std::ostream &operator<<(std::ostream &out, const object_t &obj) {
    out << '{';
    std::size_t count{0};

    for (const auto &pair : obj) {
      if (count > 0) {
        out << ", ";
      }
      out << pair.first << " : " << pair.second;
      ++count;
    }
    out << '}';
    return out;
  }

  friend std::ostream &operator<<(std::ostream &out, const Json &js) {
    switch (js.mType) {
    case Type::String:
      out << *js.mStrPtr;
      break;
    case Type::Null:
      out << js.mNull;
      break;
    case Type::Object:
      out << *js.mObjectPtr;
      break;
    default:
      out << "Unimplemented";
    }
    return out;
  }

private:
  void assertObject() const {
    if (mType != Type::Object) {
      throw std::runtime_error{"Current json type is not Type::Object"};
    }
  }
  void deleteCurrent() {
    switch (mType) {
    case Type::String:
      delete mStrPtr;
      break;
    case Type::Null:
      break;
    case Type::Object:
      delete mObjectPtr;
      break;
    }
  }

  Type mType{Type::Null};
  union {
    Null mNull{};
    string_t *mStrPtr;
    object_t *mObjectPtr;
  };
};

} // namespace json

#endif
