#ifndef JSON_CODEC_UTF8CODEC_H
#define JSON_CODEC_UTF8CODEC_H

#include "cassert"
#include "icodec.h"
#include "rune.h"
#include "utf8char.h"

#include <vector>

namespace json {
namespace codec {
// https://en.wikipedia.org/wiki/UTF-8
class UTF8Codec final : public ICodec {
public:
  bool isValid(const char *begin, const char *end) {
    assert(isValidSequence(begin, end));
    return (isSingleByteChar(begin) || isTwoByteChar(begin) ||
            isThreeByteChar(begin) || isFourByteChar(begin) ||
            isInBetweenChar(begin));
  }
  std::size_t getSizeOfNextCharacter(const char *begin, const char *end) {
    assert(isValidSequence(begin, end));
    if (isSingleByteChar(begin)) {
      return 1;
    } else if (isTwoByteChar(begin)) {
      return 2;
    } else if (isThreeByteChar(begin)) {
      return 3;
    } else if (isFourByteChar(begin)) {
      return 4;
    } else {
      // ???
      return 0;
    }
  }

  UTF8Char fromRune(Rune) { return UTF8Char(nullptr, 0); }

  Rune toRune(UTF8Char ch) {
    Rune rune{0};
    char buf[ch.size()];
    ch.checkSizeAndCopyTo(buf, ch.size());

    switch (ch.size()) {
    case 4:
      rune = (buf[0] & 0b0000'0111);
      rune = (rune << 6) | (buf[1] & 0b0011'1111);
      rune = (rune << 6) | (buf[2] & 0b0011'1111);
      rune = (rune << 6) | (buf[3] & 0b0011'1111);
      return rune;
    case 3:
      rune = (buf[0] & 0b0000'1111);
      rune = (rune << 6) | (buf[1] & 0b0011'1111);
      rune = (rune << 6) | (buf[2] & 0b0011'1111);
      return rune;
    case 2:
      rune = (buf[0] & 0b0001'1111);
      rune = (rune << 6) | (buf[1] & 0b0011'1111);
      return rune;
    case 1:
      rune = buf[0];
      return rune;
    default:
      return rune;
    }
  }

  std::vector<char> encode(const std::vector<Rune> &toEncode) override {
    return std::vector<char>{};
  }

  std::vector<Rune> decode(const std::vector<char> &toDecode) override {
    if (toDecode.size() == 0) {
      return std::vector<Rune>{};
    }
    std::vector<Rune> decoded{};
    auto data = toDecode.data();
    const char *cur = data;
    const char *end = data + toDecode.size();

    while (cur < end) {
      if (!isValid(cur, end)) {
        throw std::runtime_error{"JsonDecodeError1: invalid utf-8 byte"};
      }
      auto len{getSizeOfNextCharacter(cur, end)};
      if (len == 0) {
        throw std::runtime_error{"JsonDecodeError2: invalid utf-8 byte"};
      }

      UTF8Char utf8char(cur, len);
      Rune rune{toRune(utf8char)};
      decoded.push_back(rune);
      cur += len;
    }
    return decoded;
  }

private:
  bool isValidSequence(const char *begin, const char *end) {
    return begin < end;
  }
  bool isSingleByteChar(const char *ch) {
    // if left most bit is zero, then ch is in ascii range.
    return !((*ch) & 0b1000'0000);
  }
  bool isTwoByteChar(const char *ch) {
    // if utf-8 char starts with 110xxxxx byte, then it is 2-byte character
    return ((*ch) & 0b1110'0000) == 0b1100'0000;
  }
  bool isThreeByteChar(const char *ch) {
    // if utf-8 char starts with 1110xxxx byte, then it is 3-byte character
    return ((*ch) & 0b1111'0000) == 0b1110'0000;
  }
  bool isFourByteChar(const char *ch) {
    // if utf-8 char starts with 11110xxx byte, then it is 4-byte character
    return ((*ch) & 0b1111'1000) == 0b1111'0000;
  }
  bool isInBetweenChar(const char *ch) {
    return ((*ch) & 0b1100'0000) == 0b1000'0000;
  }
};

} // namespace codec
} // namespace json

#endif
