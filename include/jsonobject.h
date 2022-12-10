#ifndef JSON_JSONOBJECT_H
#define JSON_JSONOBJECT_H

#include "json.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace json {
void Json::assert_object_type() const {
  if (m_variant.type() != Type::object) {
    throw std::runtime_error{"TypeError: not a json object"};
  }
}

std::vector<Json::object_t::key_type> Json::keys() const {
  assert_object_type();
  std::vector<Json::object_t::key_type> vec;
  const Json::object_t &obj = m_variant.object();
  std::for_each(obj.cbegin(), obj.cend(),
                [&](const auto &item) { vec.push_back(item.first); });
  return vec;
}

std::vector<Json::object_t::mapped_type> Json::values() const {
  assert_object_type();
  std::vector<Json::object_t::mapped_type> vec;
  const Json::object_t &obj = m_variant.object();
  std::for_each(obj.cbegin(), obj.cend(),
                [&](const auto &item) { vec.push_back(item.second); });
  return vec;
}

std::vector<Json::object_t::value_type> Json::items() const {
  assert_object_type();
  std::vector<Json::object_t::value_type> vec;
  const Json::object_t &obj = m_variant.object();
  std::for_each(obj.cbegin(), obj.cend(),
                [&](const auto &item) { vec.push_back(item); });
  return vec;
}

std::ostream &operator<<(std::ostream &out, const Json::object_t &obj) {
  out << '{';
  std::size_t index = 0;
  for (const auto &item : obj) {
    if (index > 0) {
      out << ", ";
    }
    out << item;
    ++index;
  }
  out << '}';
  return out;
}

std::ostream &operator<<(std::ostream &out,
                         const Json::object_t::value_type &item) {
  out << item.first << ": " << item.second;
  return out;
}

Json::object_t &&Json::moveObject() {
  assert_object_type();
  return m_variant.extract_object();
}
Json::object_t Json::copyObject() const {
  assert_object_type();
  return m_variant.object();
}

bool Json::isObject() const { return (m_variant.type() == Type::object); }

const Json::object_t::mapped_type &
Json::operator[](const object_t::key_type &key) const {

  assert_object_type();

  const auto &obj = m_variant.object();
  auto result = obj.find(key);

  if (result == obj.cend()) {
    throw std::runtime_error{std::string{"KeyError: "} + std::string{key}};
  }

  return result->second;
}

void Json::update(std::vector<Json::object_t::value_type> pairs) {
  Json::object_t &obj = m_variant.object();
  obj.insert(pairs.begin(), pairs.end());
}
} // namespace json

#endif
