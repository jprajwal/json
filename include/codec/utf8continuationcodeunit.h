#ifndef JSON_CODEC_UTF8CONTINUATIONCODEUNIT_H
#define JSON_CODEC_UTF8CONTINUATIONCODEUNIT_H

#include "codec/continuationcodeunit.h"

namespace json {
namespace codec {

template <typename Traits>
class ContinuationCodeUnit<char, Traits>
    : public ContinuationCodeUnitBase<char, Traits> {
public:
  using ContinuationCodeUnitBase<char, Traits>::ContinuationCodeUnitBase;

  bool isValid() { return (this->mData & 0b1100'0000) == 0b1000'0000; }
  std::size_t payloadSize() { return 6; }
  typename Traits::int_type payload() { return 0b0011'1111 & this->mData; }
};
} // namespace codec
} // namespace json

#endif