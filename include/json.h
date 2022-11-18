#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "null.h"
#include "object.h"
#include "string.h"

#include <initializer_list>
#include <iostream>
#include <memory>

namespace json {
class Object;
class Json;
using PairOfStrings = std::pair<String, String>;
using PairOfStringAndJson = std::pair<String, Json>;

class Json {
public:
  enum class Type {
    Object,
    String,
    Null,
  };

public:
  // default json object
  Json();

  // json string
  Json(std::string str);
  Json(const char *str);

  // json object
  Json(std::initializer_list<PairOfStringAndJson> pairs);

  // TODO: copy and move constructors required?
  Json(const Json &);
  Json(const Json &&);

  // common operations on json
  // TODO: serializer? or seperate interface needed?
  friend std::ostream &operator<<(std::ostream &out, const Json &js);
  friend bool operator==(const Json &, const Json &);
  friend bool operator!=(const Json &, const Json &);

  // object related operations
  // 1. object access interface
  Json &operator[](const String &key);

  // Access methods
  const Type type() const;
  const String innerString() const;

private:
  Type mType{Type::Null};
  Null mNull{};
  std::shared_ptr<Object> mObjectPtr{};
  String mString{};

private:
  Json &operator=(const Json &);
  Json &operator=(const Json &&);
};

} // namespace json

#endif
