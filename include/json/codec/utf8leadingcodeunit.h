#ifndef JSON_CODEC_UTF8LEADINGCODEUNIT_H
#define JSON_CODEC_UTF8LEADINGCODEUNIT_H
#include "json/codec/leadingcodeunit.h"
#include <string>

namespace json {
namespace codec {
// namespace utf8 {

template <typename Traits>
class LeadingCodeUnit<char, Traits>
    : public json::codec::LeadingCodeUnitBase<char, Traits> {
public:
  using LeadingCodeUnitBase<char, Traits>::LeadingCodeUnitBase;

  std::size_t continuationUnitsCount() {
    if (isSingleByteChar()) {
      return 0;
    } else if (isTwoByteChar()) {
      return 1;
    } else if (isThreeByteChar()) {
      return 2;
    } else if (isFourByteChar()) {
      return 3;
    } else {
      return 0;
    }
  }

  bool isValid() {
    return (isSingleByteChar() || isTwoByteChar() || isThreeByteChar() ||
            isFourByteChar()) &&
           (Traits::eof() != this->mData);
  }

  std::size_t payloadSize() {
    if (isSingleByteChar()) {
      return 7;
    } else if (isTwoByteChar()) {
      return 5;
    } else if (isThreeByteChar()) {
      return 4;
    } else if (isFourByteChar()) {
      return 3;
    } else {
      return 0;
    }
  }

  typename Traits::int_type payload() {
    if (isSingleByteChar()) {
      return this->mData & 0b0111'1111;
    } else if (isTwoByteChar()) {
      return this->mData & 0b0001'1111;
    } else if (isThreeByteChar()) {
      return this->mData & 0b0000'1111;
    } else if (isFourByteChar()) {
      return this->mData & 0b0000'0111;
    } else {
      return 0;
    }
  }

private:
  bool isSingleByteChar() {
    // if left most bit is zero, then ch is in ascii range.
    return !((this->mData) & 0b1000'0000);
  }
  bool isTwoByteChar() {
    // if utf-8 char starts with 110xxxxx byte, then it is 2-byte character
    return ((this->mData) & 0b1110'0000) == 0b1100'0000;
  }
  bool isThreeByteChar() {
    // if utf-8 char starts with 1110xxxx byte, then it is 3-byte character
    return ((this->mData) & 0b1111'0000) == 0b1110'0000;
  }
  bool isFourByteChar() {
    // if utf-8 char starts with 11110xxx byte, then it is 4-byte character
    return ((this->mData) & 0b1111'1000) == 0b1111'0000;
  }
};
// } // namespace utf8
} // namespace codec
} // namespace json

#endif
