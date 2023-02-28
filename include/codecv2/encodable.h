#ifndef JSON_CODECV2_ENCODABLE_H
#define JSON_CODECV2_ENCODABLE_H

#include "codec/rune.h"

#include <string>
#include <vector>

namespace json {
namespace codecv2 {

template <typename CharT> class Encodable {
public:
  // virtual CharT encodeOne(codec::Rune) const = 0;
  virtual std::basic_string<CharT>
  encode(const std::vector<codec::Rune> &) const = 0;
};

} // namespace codecv2
} // namespace json

#endif
