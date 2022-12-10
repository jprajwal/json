#ifndef JSON_JSONSTRING_H
#define JSON_JSONSTRING_H

#include "json.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace json {
Json::string_t Json::copyString() const {

  if (m_variant.type() != Type::string) {
    throw std::runtime_error{"TypeError: not a json string"};
  }
  return m_variant.str();
}

Json::string_t &&Json::moveString() {
  if (m_variant.type() != Type::string) {
    throw std::runtime_error{"TypeError: not a json string"};
  }

  return m_variant.extract_str();
}

std::vector<Json::string_t::value_type> Json::chars() const {
  if (m_variant.type() != Type::string) {
    throw std::runtime_error{"TypeError: not a json string"};
  }
  std::vector<Json::string_t::value_type> vec;
  Json::string_t &str = m_variant.str();
  std::copy(str.cbegin(), str.cend(), std::back_inserter(vec));
  return vec;
}

bool Json::isString() const { return (m_variant.type() == Type::string); }

} // namespace json
#endif
