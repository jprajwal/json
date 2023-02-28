
#ifndef JSON_CODECV2_UTF16_CCU_H
#define JSON_CODECV2_UTF16_CCU_H

#include "codecv2/ccutrait.h"

namespace json {
namespace codecv2 {

class UTF16CCU : CCUTrait<char16_t> {

public:
  using CCUTrait<char16_t>::CCUTrait;
  bool isValid() const override { return (this->mData >> 10) == 0xDC00; }

  std::size_t payloadSize() const override { return 10; }

  int_type payload() const override { return this->mData << 6; }
};

} // namespace codecv2
} // namespace json

#endif
