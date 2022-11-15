#ifndef JSON_CODEC_ICODEC_H
#define JSON_CODEC_ICODEC_H

#include "rune.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace json {
namespace codec {

template <typename CharT, typename Traits> class CodecTraitBase {
public:
  std::basic_string<CharT, Traits>
  encode(const std::basic_string<CharT, Traits> &) {
    throw std::runtime_error{"Operation not implemented"};
  }

  std::vector<Rune> decode(const std::basic_string<CharT, Traits> &) {
    throw std::runtime_error{"Operation not implemented"};
  }
};

template <typename CharT, typename Traits = std::char_traits<CharT>>
class UTF8Codec : public CodecTraitBase<CharT, Traits> {};

} // namespace codec
} // namespace json

#endif