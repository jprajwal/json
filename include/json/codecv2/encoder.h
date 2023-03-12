#ifndef JSON_CODECV2_ENCODER_H
#define JSON_CODECV2_ENCODER_H

#include "json/codecv2/encodable.h"
#include "json/test.h"
#include <string>
namespace json {
namespace codecv2 {

class UTF8Encoder : public Encodable<char> {
public:
  std::basic_string<char>
  encode(const std::vector<codec::Rune> &runes) const override {
    std::basic_string<char> result;

    for (auto rune : runes) {
      std::size_t ccuCount = 0;
      typename std::char_traits<char>::int_type lcu = 0;
      if (rune <= 0x7f) {
        ccuCount = 0;
        lcu = lcu | (0b01111111 & rune);
      } else if (rune <= 0x07FF) {
        // json::log << "rune is <= 0x7ff" << std::endl;
        ccuCount = 1;
        lcu = 0b11000000 | ((rune >> 6) & 0b00011111);
      } else if (rune <= 0xFFFF) {
        // json::log << "rune is <= 0xffff" << std::endl;
        ccuCount = 2;
        lcu = 0b11100000 | ((rune >> 12) & 0b00001111);
      } else if (rune <= 0x10FFFF) {
        ccuCount = 3;
        lcu = 0b11110000 | ((rune >> 18) & 0b00000111);
      }

      for (std::size_t i = 0; i < ccuCount; ++i) {
        typename std::char_traits<char>::int_type ccu = 0;
        ccu = 0b10000000 | ((rune >> 6 * i) & 0b00111111);
        result.insert(0, 1, static_cast<char>(ccu));
      }

      result.insert(0, 1, static_cast<char>(lcu));
    }
    return result;
  }
};

} // namespace codecv2
} // namespace json

#endif
