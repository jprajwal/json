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

template <typename CharT = char> class JsonT {
public:
  using string_t = std::basic_string<CharT>;
  using object_t = std::map<string_t, JsonT>;

  // defaults
  JsonT() : mNull{} {}

  // Json strings
  JsonT(const char *str) : mType{Type::String}, mStrPtr{new string_t{str}} {}
  JsonT(const string_t &str)
      : mType{Type::String}, mStrPtr{new string_t{str}} {}
  JsonT(string_t &&str)
      : mType{Type::String}, mStrPtr{new string_t{std::move(str)}} {}

  // Json Objects
  JsonT(std::initializer_list<std::pair<const string_t, JsonT>> pairs)
      : mType{Type::Object}, mObjectPtr{new object_t{pairs}} {}

  // Copy constructor
  JsonT(const JsonT &other) : mType{other.mType}, mNull{} {
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
  JsonT(JsonT &&other) noexcept : mType{other.mType}, mNull{} {
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
  JsonT &operator=(const JsonT &other) {
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
  JsonT &operator=(JsonT &&other) noexcept {
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

  ~JsonT() { deleteCurrent(); }

  Type type() const { return mType; }

  bool operator==(const JsonT &other) const {
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

  JsonT &operator[](const string_t &key) {
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

  friend std::ostream &operator<<(std::ostream &out, const JsonT &js) {
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
  void assertObject() {
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
  template <typename T> friend class str_iter;
  template <typename T> friend class obj_iter;

  Type mType{Type::Null};
  union {
    Null mNull{};
    string_t *mStrPtr;
    object_t *mObjectPtr;
  };
};

// Typedefs
typedef JsonT<char> Json;
typedef JsonT<char16_t> WJson;
typedef JsonT<char32_t> QJson;

template <typename JsonT = Json> class str_iter {
public:
  explicit str_iter(const JsonT &js) : mRef{js} {
    if (mRef.type() != Type::String) {
      throw std::runtime_error{"str_iter is supported only for json strings"};
    }
  }
  typename JsonT::string_t::const_iterator begin() const {
    return mRef.mStrPtr->cbegin();
  }

  typename JsonT::string_t::const_iterator end() const {
    return mRef.mStrPtr->cend();
  }

private:
  const JsonT &mRef;
};

template <typename JsonT = Json> class obj_iter {
public:
  explicit obj_iter(const JsonT &js) : mRef{js} {
    if (mRef.type() != Type::Object) {
      throw std::runtime_error{"obj_iter is supported only for json objects"};
    }
  }
  typename JsonT::object_t::const_iterator begin() const {
    return mRef.mObjectPtr->cbegin();
  }

  typename JsonT::object_t::const_iterator end() const {
    return mRef.mObjectPtr->cend();
  }

  typename JsonT::object_t::iterator begin() {
    return mRef.mObjectPtr->begin();
  }

  typename JsonT::object_t::iterator end() { return mRef.mObjectPtr->end(); }

private:
  const JsonT &mRef;
};

} // namespace json

#endif
