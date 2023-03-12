
#ifndef JSON_CODECV2_UTF16_CCU_H
#define JSON_CODECV2_UTF16_CCU_H

#include "json/codecv2/ccutrait.h"

namespace json {
namespace codecv2 {

class UTF16CCU : CCUTrait<char16_t> {

public:
  using CCUTrait<char16_t>::CCUTrait;
  bool isValid() const override { return (this->mData >> 10) == 0b00110111; }

  std::size_t payloadSize() const override { return 10; }

  int_type payload() const override { return 0x03FF & this->mData; }
};

} // namespace codecv2
} // namespace json

#endif
