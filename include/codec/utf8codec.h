#ifndef JSON_CODEC_UTF8CODEC_H
#define JSON_CODEC_UTF8CODEC_H

#include "codec/utf8/continuationcodeunit.h"
#include "codec/utf8/leadingcodeunit.h"
#include "icodec.h"
#include "rune.h"

#include <stdexcept>
#include <vector>

namespace json {
namespace codec {
template <typename Traits>
class UTF8Codec<char, Traits> : public CodecTraitBase<char, Traits> {
public:
  std::basic_string<char, Traits>
  encode(const std::basic_string<char, Traits> &) {
    throw std::runtime_error{"Operation not implemented"};
  }

  std::vector<Rune> decode(const std::basic_string<char, Traits> &toDecode) {
    std::vector<Rune> decoded{};
    auto cur{toDecode.begin()};
    auto end{toDecode.end()};
    std::size_t index{0};
    while (cur < end) {
      LeadingCodeUnit<char> lcu{*cur};
      if (!lcu.isValid()) {
        throw std::runtime_error{std::string{"UTF8DecodeError: position: "} +
                                 std::to_string(index)};
      }

      auto continuationUnitsCount{lcu.continuationUnitsCount()};

      if (index + continuationUnitsCount > toDecode.length()) {
        throw std::runtime_error{std::string{"UTF8DecodeError: position: "} +
                                 std::to_string(index)};
      }
      Rune rune{lcu.payload()};
      ++cur;
      ++index;

      std::size_t subIndex{0};
      while (subIndex < continuationUnitsCount) {
        ContinuationCodeUnit<char> ccu{*cur};
        if (!ccu.isValid()) {
          throw std::runtime_error{std::string{"UTF8DecodeError: position: "} +
                                   std::to_string(index + subIndex)};
        }
        rune = (rune << ccu.payloadSize()) | ccu.payload();
        ++cur;
        ++subIndex;
      }
      decoded.push_back(rune);
    }
    return decoded;
  }
};
} // namespace codec
} // namespace json

#endif
