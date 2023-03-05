#ifndef JSON_CODECV2_DECODER_H
#define JSON_CODECV2_DECODER_H

#include "codec/rune.h"
#include "codecv2/decodable.h"
#include "codecv2/utf16_ccu.h"
#include "codecv2/utf16_lcu.h"
#include "codecv2/utf8_ccu.h"
#include "codecv2/utf8_lcu.h"
#include "test.h"

#include <bitset>
#include <iterator>

namespace json {
namespace codecv2 {

template <typename CharT, typename LCU, typename CCU>
class Decoder : public Decodable<CharT> {
public:
  codec::Rune decodeOne(
      typename std::basic_string<CharT>::const_iterator &cur,
      typename std::basic_string<CharT>::const_iterator &end) const override {

    LCU lcu = *cur;
    json::log << "lcu: " << std::hex << *cur << ", " << std::bitset<16>(*cur)
              << std::endl;
    std::size_t index = 0;
    if (!lcu.isValid()) {
      throw index;
    }

    auto ccuCount = lcu.continuationUnitCount();
    json::log << "ccuCount: " << ccuCount << std::endl;
    if (static_cast<signed long>(index + ccuCount) > std::distance(cur, end)) {
      json::log << "iterator distance is invalid" << std::endl;
      throw index;
    }

    codec::Rune rune = static_cast<codec::Rune>(lcu.payload());
    ++cur;
    ++index;
    json::log << "lcu payload: " << std::hex << rune << ", "
              << std::bitset<16>(rune) << std::endl;

    std::size_t subIndex = 0;
    while (subIndex < ccuCount) {
      CCU ccu{*cur};
      json::log << "ccu: " << std::hex << *cur << ", " << std::bitset<16>(*cur)
                << std::endl;
      if (!ccu.isValid()) {
        json::log << "ccu is invalid" << std::endl;
        throw index + subIndex;
      }
      rune =
          (rune << ccu.payloadSize()) | static_cast<codec::Rune>(ccu.payload());
      ++cur;
      ++subIndex;
    }
    json::log << "rune: " << std::hex << rune << ", " << std::bitset<32>(rune)
              << std::endl;
    return rune;
  }

  std::vector<codec::Rune>
  decode(const std::basic_string<CharT> &toDecode) const override {
    std::vector<codec::Rune> decoded{};
    auto cur = toDecode.begin();
    auto end = toDecode.end();
    std::size_t index = 0;
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
};

typedef Decoder<char, UTF8LCU, UTF8CCU> UTF8Decoder;

class UTF16Decoder : public Decoder<char16_t, UTF16LCU, UTF16CCU> {
  codec::Rune
  decodeOne(typename std::basic_string<char16_t>::const_iterator &cur,
            typename std::basic_string<char16_t>::const_iterator &end)
      const override {
    auto rune =
        this->Decoder<char16_t, UTF16LCU, UTF16CCU>::decodeOne(cur, end);
    rune = rune + 0x10000;
    return rune;
  }
};

} // namespace codecv2
} // namespace json

#endif
