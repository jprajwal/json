#ifndef JSON_JSONCOMMON_H
#define JSON_JSONCOMMON_H

#include "json.h"

#include <iostream>

namespace json {
std::ostream &operator<<(std::ostream &out, const Json &jsn) {
  switch (jsn.m_variant.type()) {
  case Type::string:
    out << jsn.m_variant.str();
    break;
  case Type::null:
    out << jsn.m_variant.null();
    break;
  case Type::object:
    out << jsn.m_variant.object();
    break;
  case Type::integer:
    out << jsn.m_variant.integer();
    break;
  default:
    out << "Not Implemented";
  }
  return out;
}
} // namespace json

#endif
