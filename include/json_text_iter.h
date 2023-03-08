#ifndef JSON_JSON_TEXT_EDITOR_H
#define JSON_JSON_TEXT_EDITOR_H

#include <iterator>
#include <string>
#include <string_view>

namespace json {

using namespace std::literals;

class JsonTextIter {
public:
  JsonTextIter() = default;
  JsonTextIter(const std::string_view str)
      : m_str{str}, m_iter{m_str.begin()}, m_index{0}, m_lineno{1},
        m_last_lf{} {}
  JsonTextIter(const JsonTextIter &) = default;
  JsonTextIter(JsonTextIter &&) = default;

public:
  using difference_type = std::ptrdiff_t;
  using value_type = std::string_view::value_type;
  using pointer = void;
  using reference = void;
  using iterator_category = std::forward_iterator_tag;

public:
  value_type eof() const { return value_type(); }

  value_type operator*() {
    if (hasOverflowed()) {
      return eof();
    }
    return *m_iter;
  }

  JsonTextIter &operator++() {
    ++m_iter;
    ++m_index;

    if (m_iter < m_str.cend() && *m_iter == '\n') {
      ++m_lineno;
      m_last_lf = m_index;
    }
    return *this;
  }

  JsonTextIter operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  bool operator==(const JsonTextIter &other) const {
    return m_iter == other.m_iter;
  }

  JsonTextIter begin() { return JsonTextIter{m_str}; }

  JsonTextIter end() {
    JsonTextIter iter{m_str};
    iter.m_iter = m_str.cend();
    return iter;
  }

  // bool operator<(const JsonTextIter &other) const {
  //   return m_iter < other.m_iter;
  // }

  // bool operator>(const JsonTextIter &other) const {
  //   return !(operator<(other));
  // }
  //
  value_type peek(std::size_t offset) {
    if ((offset < 0) || (m_iter + offset) >= m_str.cend()) {
      return eof();
    }

    return *(m_iter + offset);
  }

  std::string_view peek_n(std::size_t offset, std::size_t count) {
    if (offset < 0 || count < 1)
      return ""sv;

    return m_str.substr(std::min(m_index + offset, m_str.length() - 1), count);
  }

  std::basic_string<value_type> take_n(std::size_t n) {
    std::basic_string<value_type> result;

    for (std::size_t i = 0; i < n; ++i) {
      if (m_iter < m_str.cend()) {
        result.push_back(*(m_iter));
        ++(*this);
      }
    }
    return result;
  }

  bool hasOverflowed() const { return m_index >= m_str.length(); }

  std::size_t index() const { return m_index; }

  std::size_t lineno() const { return m_lineno; }

  std::string_view::difference_type remaining() const {
    return m_str.cend() - m_iter - 1;
  }

  std::size_t column() const {
    return std::min(m_str.length() - 1, m_index) - m_last_lf;
  }

private:
  std::string_view m_str{};
  std::string_view::const_iterator m_iter{};
  std::size_t m_index{};
  std::size_t m_lineno{1};
  std::size_t m_last_lf{};
};

} // namespace json

#endif
