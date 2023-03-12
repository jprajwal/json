#ifndef JSON_CODECV2_DECODABLE_H
#define JSON_CODECV2_DECODABLE_H

#include "json/codec/rune.h"

#include <string>
#include <vector>

namespace json {
namespace codecv2 {

template <typename CharT> class Decodable {
public:
  virtual codec::Rune
  decodeOne(typename std::basic_string<CharT>::const_iterator &,
            typename std::basic_string<CharT>::const_iterator &) const = 0;

  virtual std::vector<codec::Rune>
  decode(const std::basic_string<CharT> &) const = 0;
};

} // namespace codecv2
} // namespace json

#endif
