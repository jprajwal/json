#ifndef JSON_CODECV2_UTF8_CCU_H
#define JSON_CODECV2_UTF8_CCU_H

#include "codecv2/ccutrait.h"

namespace json {
namespace codecv2 {

class UTF8CCU : CCUTrait<char> {

public:
  using CCUTrait<char>::CCUTrait;
  bool isValid() const override {
    return (this->mData & 0b1100'0000) == 0b1000'0000;
  }

  std::size_t payloadSize() const override { return 6; }

  int_type payload() const override { return 0b0011'1111 & this->mData; }
};

} // namespace codecv2
} // namespace json

#endif
