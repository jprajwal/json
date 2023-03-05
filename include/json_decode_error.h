#ifndef JSON_JSON_DECODE_ERROR_H
#define JSON_JSON_DECODE_ERROR_H

#include <cstdio>
#include <exception>
#include <string>

namespace json {
class JsonDecodeError : std::exception {
private:
  std::size_t m_pos{};
  std::string m_description{};
  std::string m_msg{};

public:
  JsonDecodeError(std::size_t pos, const std::string &str)
      : m_pos{pos}, m_description{str} {
    std::sprintf(m_msg.data(), "column %lu: %s", m_pos, str.c_str());
  }

  JsonDecodeError(std::size_t pos, const char *str)
      : m_pos{pos}, m_description{str} {
    std::sprintf(m_msg.data(), "column %lu: %s", m_pos, str);
  }

  const char *what() const noexcept override { return m_msg.c_str(); }
};
} // namespace json

#endif
