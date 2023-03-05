#ifndef JSON_CODECV2_UTF16_LCU_H
#define JSON_CODECV2_UTF16_LCU_H

#include "codecv2/lcutrait.h"

namespace json {
namespace codecv2 {
class UTF16LCU : LCUTrait<char16_t> {

public:
  using LCUTrait<char16_t>::LCUTrait;

  std::size_t continuationUnitCount() const override {
    if (isTwoByteChar()) {
      return 0;
    } else if (isFourByteChar()) {
      return 1;
    } else {
      return 0;
    }
  }
  bool isValid() const override {
    return (isTwoByteChar() || isFourByteChar()) &&
           (CharTraits::eof() != this->mData);
  }
  std::size_t payloadSize() const override {
    if (isTwoByteChar()) {
      return 16;
    } else if (isFourByteChar()) {
      return 10;
    } else {
      return 0;
    }
  }
  int_type payload() const override { return 0x03FF & mData; }

private:
  bool isTwoByteChar() const {
    return !isFourByteChar() && this->mData <= 0xFFFF;
  }

  bool isFourByteChar() const {
    return this->mData >= 0xD800 && this->mData <= 0xDFFF;
  }
};

} // namespace codecv2
} // namespace json

#endif
