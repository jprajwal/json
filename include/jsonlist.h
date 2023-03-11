#ifndef JSON_JSONLIST_H
#define JSON_JSONLIST_H

#include "json.h"

#include <iterator>

namespace json {
class Json::list_iterator {

public:
  using difference_type = std::ptrdiff_t;
  using value_type = Json &;
  using pointer = void;
  using reference = void;
  using iterator_category = std::input_iterator_tag;

  list_iterator(const Json &js) : m_ref{js}, m_cur{js.toList().begin()} {}

  list_iterator begin() const {
    list_iterator iter{m_ref};
    iter.set_begin();
    return iter;
  }

  list_iterator end() const {
    list_iterator iter{m_ref};
    iter.set_end();
    return iter;
  }

  bool operator==(const list_iterator other) const {
    return this->m_cur == other.m_cur;
  }

  bool operator!=(const list_iterator other) const { return !(*this == other); }

  list_iterator &operator++() {
    auto last = end();
    if (*this == last) {
      return *this;
    }

    ++m_cur;
    return *this;
  }

  list_iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  value_type operator*() const { return *m_cur; }

private:
  void set_begin() { m_cur = m_ref.m_variant.list().begin(); }
  void set_end() { m_cur = m_ref.m_variant.list().end(); }

private:
  const Json &m_ref;
  list_t::iterator m_cur;
};

Json::list_iterator Json::list_iter() const { return list_iterator{*this}; }

void Json::push_back(list_t::value_type &&value) {
  assert_list_type();
  auto &list = m_variant.list();
  list.push_back(std::forward<list_t::value_type>(value));
}

Json::list_t::value_type &Json::operator[](const std::size_t index) {
  assert_list_type();
  auto &list = m_variant.list();
  return list[index];
}

std::ostream &operator<<(std::ostream &out, const Json::list_t &list) {
  out << '[';
  std::size_t index = 0;
  for (auto &item : list) {
    if (index > 0) {
      out << ", ";
    }
    ++index;

    out << item;
  }
  out << ']';
  return out;
}

} // namespace json

#endif