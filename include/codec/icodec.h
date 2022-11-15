#ifndef JSON_CODEC_ICODEC_H
#define JSON_CODEC_ICODEC_H

#include "rune.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace json {
namespace codec {

template <typename CharT, typename Traits = std::char_traits<CharT>>
class CodecTraitBase {
public:
  std::basic_string<CharT, Traits>
  encode(const std::basic_string<CharT, Traits> &) {
    throw std::runtime_error{"Operation not implemented"};
  }

  std::vector<Rune> decode(const std::basic_string<CharT, Traits> &toDecode) {
    std::vector<Rune> decoded{};
    auto cur{toDecode.begin()};
    auto end{toDecode.end()};
    std::size_t index{0};
    while (cur < end) {
      LeadingCodeUnit<CharT> lcu{*cur};
      if (!lcu.isValid()) {
        throw std::runtime_error{std::to_string(index)};
      }

      auto continuationUnitsCount{lcu.continuationUnitsCount()};

      if (index + continuationUnitsCount > toDecode.length()) {
        throw std::runtime_error{std::to_string(index)};
      }
      Rune rune{lcu.payload()};
      ++cur;
      ++index;

      std::size_t subIndex{0};
      while (subIndex < continuationUnitsCount) {
        ContinuationCodeUnit<CharT> ccu{*cur};
        if (!ccu.isValid()) {
          throw std::runtime_error{std::to_string(index + subIndex)};
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

template <typename CharT, typename Traits = std::char_traits<CharT>>
class UTF8CodecBase : public CodecTraitBase<CharT, Traits> {};

} // namespace codec
} // namespace json

#endif