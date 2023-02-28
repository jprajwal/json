#ifndef JSON_CODECV2_UTF8_LCU_H
#define JSON_CODECV2_UTF8_LCU_H

#include "codecv2/lcutrait.h"

namespace json {
namespace codecv2 {

class UTF8LCU : LCUTrait<char> {
public:
  using LCUTrait<char>::LCUTrait;

  std::size_t continuationUnitCount() const override {
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
  bool isValid() const override {
    return (isSingleByteChar() || isTwoByteChar() || isThreeByteChar() ||
            isFourByteChar()) &&
           (CharTraits::eof() != this->mData);
  }
  std::size_t payloadSize() const override {
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
  int_type payload() const override {
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
  bool isSingleByteChar() const {
    // if left most bit is zero, then ch is in ascii range.
    return !((this->mData) & 0b1000'0000);
  }
  bool isTwoByteChar() const {
    // if utf-8 char starts with 110xxxxx byte, then it is 2-byte character
    return ((this->mData) & 0b1110'0000) == 0b1100'0000;
  }
  bool isThreeByteChar() const {
    // if utf-8 char starts with 1110xxxx byte, then it is 3-byte character
    return ((this->mData) & 0b1111'0000) == 0b1110'0000;
  }
  bool isFourByteChar() const {
    // if utf-8 char starts with 11110xxx byte, then it is 4-byte character
    return ((this->mData) & 0b1111'1000) == 0b1111'0000;
  }
};

} // namespace codecv2
} // namespace json

#endif
