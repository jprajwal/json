#ifndef JSON_CODEC_UTF16CODEC_H
#define JSON_CODEC_UTF16CODEC_H

#include "ichar.h"
#include "icodec.h"
#include "test.h"

#include <cstdint>
#include <stdexcept>
#include <string>

namespace json {
namespace codec {

class UTF16Codec : public CodecTraitBase<char16_t> {
public:
  ICharBase<char16_t> encodeSingle(Rune rune) {
    if (!rune.isValid()) {
      throw std::runtime_error{std::string{"UTF16CharEncodeError"}};
    }
    if (rune.isBMP()) {
      ICharBase<char16_t> result(1);
      result.at(0) = static_cast<char16_t>(static_cast<std::uint32_t>(rune));
      return result;
    }
    const std::uint16_t temp{static_cast<std::uint16_t>(rune - 0x10000)};
    json::log << std::hex << "temp: " << temp << std::endl;
    const std::uint16_t hs{static_cast<std::uint16_t>((temp >> 10) + 0xD800)};
    json::log << std::hex << "hs: " << hs << std::endl;
    const std::uint16_t ls{static_cast<std::uint16_t>((temp & 0x3FF) + 0xDC00)};
    json::log << std::hex << "ls: " << ls << std::endl;
    ICharBase<char16_t> ch(2);
    ch.at(0) = static_cast<char16_t>(hs);
    ch.at(1) = static_cast<char16_t>(ls);
    return ch;
  }
};

} // namespace codec
} // namespace json

#endif
