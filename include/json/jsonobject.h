#ifndef JSON_JSONOBJECT_H
#define JSON_JSONOBJECT_H

#include "json/json.h"

#include <algorithm>
#include <cstddef>
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

class Json::key_iterator {
public:
  using difference_type = std::ptrdiff_t;
  using value_type = const object_t::key_type &;
  using pointer = void;
  using reference = void;
  using iterator_category = std::input_iterator_tag;

  key_iterator(const Json &jsn) : m_ref{jsn}, m_cur{} {}

  key_iterator begin() const {
    key_iterator iter{m_ref};
    iter.set_begin();
    return iter;
  }

  key_iterator end() const {
    key_iterator iter{m_ref};
    iter.set_end();
    return iter;
  }

  bool operator==(const key_iterator other) const {
    return this->m_cur == other.m_cur;
  }

  bool operator!=(const key_iterator other) const { return !(*this == other); }

  key_iterator &operator++() {
    auto last = end();
    if (*this == last) {
      return *this;
    }

    ++m_cur;
    return *this;
  }

  key_iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  value_type operator*() const { return m_cur->first; }

private:
  void set_begin() { m_cur = m_ref.m_variant.object().begin(); }
  void set_end() { m_cur = m_ref.m_variant.object().end(); }

private:
  const Json &m_ref;
  object_t::iterator m_cur;
};

class Json::value_iterator {
public:
  using difference_type = std::ptrdiff_t;
  using value_type = const object_t::mapped_type &;
  using pointer = void;
  using reference = void;
  using iterator_category = std::input_iterator_tag;

  value_iterator(const Json &jsn) : m_ref{jsn}, m_cur{} {}

  value_iterator begin() const {
    value_iterator iter{m_ref};
    iter.set_begin();
    return iter;
  }

  value_iterator end() const {
    value_iterator iter{m_ref};
    iter.set_end();
    return iter;
  }

  bool operator==(const value_iterator other) const {
    return this->m_cur == other.m_cur;
  }

  bool operator!=(const value_iterator other) const {
    return !(*this == other);
  }

  value_iterator &operator++() {
    auto last = end();
    if (*this == last) {
      return *this;
    }

    ++m_cur;
    return *this;
  }

  value_iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  value_type operator*() const { return m_cur->second; }

private:
  void set_begin() { m_cur = m_ref.m_variant.object().begin(); }
  void set_end() { m_cur = m_ref.m_variant.object().end(); }

private:
  const Json &m_ref;
  object_t::iterator m_cur;
};

class Json::item_iterator {
public:
  using difference_type = std::ptrdiff_t;
  using value_type = const object_t::value_type &;
  using pointer = void;
  using reference = void;
  using iterator_category = std::input_iterator_tag;

  item_iterator(const Json &jsn) : m_ref{jsn}, m_cur{} {}

  item_iterator begin() const {
    item_iterator iter{m_ref};
    iter.set_begin();
    return iter;
  }

  item_iterator end() const {
    item_iterator iter{m_ref};
    iter.set_end();
    return iter;
  }

  bool operator==(const item_iterator other) const {
    return this->m_cur == other.m_cur;
  }

  bool operator!=(const item_iterator other) const { return !(*this == other); }

  item_iterator &operator++() {
    auto last = end();
    if (*this == last) {
      return *this;
    }

    ++m_cur;
    return *this;
  }

  item_iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  value_type operator*() const { return *m_cur; }

private:
  void set_begin() { m_cur = m_ref.m_variant.object().begin(); }
  void set_end() { m_cur = m_ref.m_variant.object().end(); }

private:
  const Json &m_ref;
  object_t::iterator m_cur;
};
Json::key_iterator Json::keys() const {
  assert_object_type();
  return key_iterator{*this};
}

Json::value_iterator Json::values() const {
  assert_object_type();
  return value_iterator{*this};
}

Json::item_iterator Json::items() const {
  assert_object_type();
  return item_iterator{*this};
}

std::ostream &operator<<(std::ostream &out, const Json::object_t &obj) {
  out << '{';
  std::size_t index = 0;
  for (const auto &item : obj) {
    if (index > 0) {
      out << ", ";
    }
    out << item.first << ": " << item.second;
    ++index;
  }
  out << '}';
  return out;
}

std::ostream &operator<<(std::ostream &out,
                         const Json::object_t::value_type &item) {
  out << '(' << item.first << ", " << item.second << ')';
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
