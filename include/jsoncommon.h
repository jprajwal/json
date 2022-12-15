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

bool Json::operator==(const Json &other) const {
  if (m_variant.type() != other.type()) {
    return false;
  }

  switch (other.m_variant.type()) {
  case Type::string:
    return m_variant.str() == other.m_variant.str();
  case Type::object:
    return m_variant.object() == other.m_variant.object();
  case Type::integer:
    return m_variant.integer() == other.m_variant.integer();
  case Type::null:
    return true;
  default:
    return false;
  }
}

bool Json::operator!=(const Json &other) const { return !(*this == other); }

} // namespace json

#endif
