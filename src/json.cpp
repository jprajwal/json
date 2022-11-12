#include "json.h"

namespace json {

Json::Json() : mType{Type::Null}, mNull{}, mObjectPtr{}, mString{} {}

Json::Json(std::string str)
    : mType{Type::String}, mNull{}, mObjectPtr{}, mString{std::move(str)} {
  std::cout << "constructing json string" << std::endl;
}

Json::Json(const char *str)
    : mType{Type::String}, mNull{}, mObjectPtr{}, mString{str} {
  std::cout << "constructing json string from char*" << std::endl;
}

Json::Json(std::initializer_list<PairOfStringAndJson> pairs)
    : mType{Type::Object}, mNull{},
      mObjectPtr{std::make_shared<Object>(std::move(pairs))}, mString{} {
  std::cout << "constructing json object" << std::endl;
}

Json::Json(const Json &js) {
  std::cout << "Json Copy Constructor" << std::endl;
  this->mType = js.mType;
  this->mNull = js.mNull;
  this->mObjectPtr = js.mObjectPtr;
  this->mString = js.mString;
}

Json::Json(const Json &&js) {
  std::cout << "Json Move Constructor" << std::endl;
  this->mType = std::move(js.mType);
  this->mNull = std::move(js.mNull);
  this->mObjectPtr = std::move(js.mObjectPtr);
  this->mString = std::move(js.mString);
}

Json &Json::operator=(const Json &js) {
  std::cout << "Json Copy assignment" << std::endl;
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
  std::cout << "Json Move assignment" << std::endl;
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
  if (mObjectPtr->find(key) == mObjectPtr->cend()) {
    throw std::runtime_error(
        "keyerror: provided key does not exist in this json object");
  }
  return (*mObjectPtr)[key];
}

} // namespace json

// int main() {}