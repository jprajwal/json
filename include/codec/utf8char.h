#ifndef JSON_CODEC_UTF8_CHAR_H
#define JSON_CODEC_UTF8_CHAR_H

#include "rune.h"
#include <cassert>
#include <stdexcept>

namespace json {
namespace codec {
namespace constants {
inline constexpr std::size_t maxSizeOfUTF8Char = 4;
}

class UTF8Char final {
public:
  bool checkSizeAndCopyTo(char *buf, std::size_t size) {
    if (!isSizeOk(size)) {
      return false;
    }
    return copy(buf, mChar, size);
  }

  UTF8Char(const char *ch, std::size_t size)
      : mSize{static_cast<std::uint8_t>(size)}, mChar{0} {
    if (!isSizeOk(size)) {
      throw std::runtime_error{"invalid size provided for utf8 char"};
    }
    copy(mChar, ch, size);
  }

  const std::size_t size() const { return mSize; }

private:
  constexpr static bool isSizeOk(std::size_t size) {
    return (size <= constants::maxSizeOfUTF8Char && size > 0);
  }

  static bool copy(char *dest, const char *src, std::size_t size) {
    // memcpy(dest, src, size);
    for (std::size_t i{0}; i < size; ++i) {
      dest[i] = src[i];
    }
    return true;
  }

private:
  const std::uint8_t mSize{};
  char mChar[constants::maxSizeOfUTF8Char];
};

} // namespace codec
} // namespace json

#endif