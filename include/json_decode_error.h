#ifndef JSON_JSON_DECODE_ERROR_H
#define JSON_JSON_DECODE_ERROR_H

#include <cstdio>
#include <exception>
#include <string>

namespace json {
class JsonDecodeError : std::exception {
private:
  std::size_t m_lineno{};
  std::size_t m_col{};
  std::string m_description{};
  std::string m_msg{};

public:
  JsonDecodeError(std::size_t lineno, std::size_t col, const std::string &str)
      : m_lineno{lineno}, m_col{col}, m_description{str} {
    std::sprintf(m_msg.data(), "column %lu: %s", m_col, str.c_str());
  }

  JsonDecodeError(std::size_t lineno, std::size_t pos, const char *str)
      : m_lineno{lineno}, m_col{pos}, m_description{str} {
    std::sprintf(m_msg.data(), "column %lu: %s", m_col, str);
  }

  const char *what() const noexcept override { return m_msg.c_str(); }
};
} // namespace json

#endif
