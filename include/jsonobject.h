#ifndef JSON_JSONOBJECT_H
#define JSON_JSONOBJECT_H

#include "json.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
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

bool Json::isObject() const { return (m_variant.type() == Type::object); }

Json::object_t::mapped_type &Json::operator[](const object_t::key_type &key) {
  assert_object_type();
  auto &obj = m_variant.object();
  return obj[key];
}

Json &Json::update(std::vector<Json::object_t::value_type> pairs) {
  assert_object_type();

  for (auto &pair : pairs) {
    set(std::move(pair));
  }
  return *this;
}

Json &Json::set(object_t::value_type item) {
  assert_object_type();

  object_t &obj = m_variant.object();
  auto result = obj.find(item.first);

  if (result == obj.cend()) {
    obj.insert(std::move(item));
    return *this;
  }

  result->second = std::move(item.second);
  return *this;
}

Json::object_t::mapped_type Json::pop(const object_t::key_type &key) {
  assert_object_type();

  auto &obj = m_variant.object();
  auto result = obj.find(key);

  if (result == obj.cend()) {
    throw std::runtime_error{std::string{"KeyError: "} + std::string{key}};
  }

  Json value = std::move(result->second);

  auto count = obj.erase(key);

  if (count != 1) {
    throw std::runtime_error{
        std::string{"InternalError occurred while removing item"}};
  }

  return value;
}

bool Json::hasKey(const object_t::key_type &key) const {
  assert_object_type();
  const auto &obj = m_variant.object();
  return obj.find(key) != obj.cend();
}

bool Json::hasValue(const object_t::mapped_type &value) const {
  assert_object_type();
  const auto &obj = m_variant.object();
  auto result = std::find_if(
      obj.cbegin(), obj.cend(),
      [&](const object_t::value_type &item) { return item.second == value; });
  return result != obj.cend();
}
} // namespace json

#endif
