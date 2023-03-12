/*
#ifndef JSON_CODEC_JSONCODEC_H
#define JSON_CODEC_JSONCODEC_H

#include "codec/utf16char.h"
#include "codec/utf16codec.h"
#include "codec/utf8codec.h"
#include "test.h"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace json {
class JSONCodec {
public:
  void encode(std::ostream &result, const Json &js) {
    if (js.type() == Json::Type::String) {
      return encodeString(result, js);
    } else {
      throw std::runtime_error{"Operation not implemented"};
    }
  }
  Json decode(std::istream &);

private:
  void encodeString(std::ostream &result, const Json &js) {
    codec::UTF8Codec utf8{};
    codec::UTF16Codec utf16{};
    const std::string str{js.innerString()};
    auto runeArray{utf8.decode(str)};
    result << '"';
    std::size_t index{0};
    for (auto rune : runeArray) {
      log << std::hex << "rune: " << rune << std::endl;
      if (!rune.isValid()) {
        log << "rune is invalid" << std::endl;
        throw std::runtime_error{std::string{"JsonEncodeError: position: "} +
                                 std::to_string(index)};
      }
      if (rune.isASCII()) {
        result << static_cast<char>(rune);
      } else {
        codec::UTF16Char ch{utf16.encodeSingle(rune)};
        for (std::size_t i{0}; i < ch.length(); ++i) {
          std::stringstream stream;
          result << '\\';
          result << 'u';
          stream << std::hex << static_cast<std::uint16_t>(ch.at(i));
          result << stream.str();
        }
      }
      ++index;
    }
    result << '"';
  }
};
} // namespace json

#endif
*/