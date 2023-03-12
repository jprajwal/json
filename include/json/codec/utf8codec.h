#ifndef JSON_CODEC_UTF8CODEC_H
#define JSON_CODEC_UTF8CODEC_H

#include "rune.h"
#include "json/codec/utf8continuationcodeunit.h"
#include "json/codec/utf8leadingcodeunit.h"

#include <stdexcept>
#include <vector>

namespace json {
namespace codec {
namespace utf8 {

Rune decodeOne(std::string::const_iterator &cur,
               const std::string::const_iterator &end) {

  LeadingCodeUnit<char> lcu{*cur};
  std::size_t index{0};
  if (!lcu.isValid()) {
    throw index;
  }

  auto continuationUnitsCount{lcu.continuationUnitsCount()};

  if (static_cast<signed long>(index + continuationUnitsCount) >
      std::distance(cur, end)) {
    throw index;
  }
  Rune rune{static_cast<Rune>(lcu.payload())};
  ++cur;
  ++index;

  std::size_t subIndex{0};
  while (subIndex < continuationUnitsCount) {
    ContinuationCodeUnit<char> ccu{*cur};
    if (!ccu.isValid()) {
      throw index + subIndex;
    }
    rune = (rune << ccu.payloadSize()) | static_cast<Rune>(ccu.payload());
    ++cur;
    ++subIndex;
  }
  return rune;
}

std::vector<Rune> decode(const std::string &toDecode) {

  std::vector<Rune> decoded{};
  auto cur{toDecode.begin()};
  auto end{toDecode.end()};
  std::size_t index{0};
  while (cur < end) {
    try {
      auto rune{decodeOne(cur, end)};
      decoded.push_back(rune);
      ++index;
    } catch (std::size_t subIndex) {
      throw index + subIndex;
    }
  }
  return decoded;
}

} // namespace utf8
} // namespace codec
} // namespace json

#endif
