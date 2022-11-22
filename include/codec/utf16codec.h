#ifndef JSON_CODEC_UTF16CODEC_H
#define JSON_CODEC_UTF16CODEC_H

#include "rune.h"
#include "utf16char.h"

#include <cstdint>
#include <string>

namespace json {
namespace codec {
namespace utf16 {

UTF16Char encodeOne(Rune rune) {
  if (!rune.isValid()) {
    throw rune;
  }
  if (rune.isBMP()) {
    UTF16Char ch(1);
    ch.at(0) = static_cast<char16_t>(static_cast<std::uint16_t>(rune));
    return ch;
  }
  const std::uint16_t temp{static_cast<std::uint16_t>(rune - 0x10000)};
  const std::uint16_t hs{static_cast<std::uint16_t>((temp >> 10) + 0xD800)};
  const std::uint16_t ls{static_cast<std::uint16_t>((temp & 0x3FF) + 0xDC00)};
  UTF16Char ch(2);
  ch.at(0) = static_cast<char16_t>(hs);
  ch.at(1) = static_cast<char16_t>(ls);
  return ch;
}

} // namespace utf16
} // namespace codec
} // namespace json

#endif
