#include "json.h"

#include <stdexcept>

namespace json {

Json::Json() : mType{Type::Null}, mNull{}, mObjectPtr{}, mString{} {}

Json::Json(std::string str)
    : mType{Type::String}, mNull{}, mObjectPtr{}, mString{std::move(str)} {}

Json::Json(const char *str)
    : mType{Type::String}, mNull{}, mObjectPtr{}, mString{str} {}

Json::Json(std::initializer_list<PairOfStringAndJson> pairs)
    : mType{Type::Object}, mNull{},
      mObjectPtr{std::make_shared<Object>(std::move(pairs))}, mString{} {}

Json::Json(const Json &js) {
  this->mType = js.mType;
  this->mNull = js.mNull;
  this->mObjectPtr = js.mObjectPtr;
  this->mString = js.mString;
}

Json::Json(const Json &&js) {
  this->mType = std::move(js.mType);
  this->mNull = std::move(js.mNull);
  this->mObjectPtr = std::move(js.mObjectPtr);
  this->mString = std::move(js.mString);
}

Json &Json::operator=(const Json &js) {
  if (this == &js) {
    return *this;
  }

  this->mType = js.mType;
  this->mNull = js.mNull;
  this->mObjectPtr = js.mObjectPtr;
  this->mString = js.mString;
  return *this;
}

Json &Json::operator=(const Json &&js) {
  if (this == &js) {
    return *this;
  }

  this->mType = std::move(js.mType);
  this->mNull = std::move(js.mNull);
  this->mObjectPtr = std::move(js.mObjectPtr);
  this->mString = std::move(js.mString);
  return *this;
}

std::ostream &operator<<(std::ostream &out, const Json &js) {
  switch (js.mType) {
  case Json::Type::Object:
    out << *js.mObjectPtr;
    break;
  case Json::Type::String:
    out << js.mString;
    break;
  case Json::Type::Null:
    out << js.mNull;
    break;
  }
  return out;
}

Json &Json::operator[](const String &key) {
  if (mType != Type::Object) {
    throw std::runtime_error{
        "string indexing not supported for this json type"};
  }
  return (*mObjectPtr)[key];
}

bool operator==(const Json &lhs, const Json &rhs) {
  if (lhs.mType != rhs.mType) {
    return false;
  }
  switch (lhs.mType) {
  case Json::Type::String:
    return lhs.mString == rhs.mString;
  case Json::Type::Null:
    return true;
  case Json::Type::Object:
    return *lhs.mObjectPtr == *rhs.mObjectPtr;
  default:
    return false;
  }
}

bool operator!=(const Json &lhs, const Json &rhs) { return !(lhs == rhs); }
} // namespace json

// int main() {}