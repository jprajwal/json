#ifndef JSON_JSONSTRING_H
#define JSON_JSONSTRING_H

#include "json/json.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace json {
std::vector<Json::string_t::value_type> Json::chars() const {
  assert_string_type();
  std::vector<Json::string_t::value_type> vec;
  Json::string_t &str = m_variant.str();
  std::copy(str.cbegin(), str.cend(), std::back_inserter(vec));
  return vec;
}

bool Json::isString() const { return (m_variant.type() == Type::string); }

void Json::assert_string_type() const {

  if (!isString()) {
    throw std::runtime_error{"TypeError: not a json string"};
  }
}

} // namespace json
#endif
