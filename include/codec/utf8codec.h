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

template <> class UTF8CodecBase<char> : public CodecTraitBase<char> {
public:
  std::vector<Rune> decode(const std::basic_string<char> &toDecode) {
    try {
      auto decoded{CodecTraitBase::decode(toDecode)};
      return decoded;
    } catch (std::runtime_error &error) {
      throw std::runtime_error{std::string{"UTF8DecodeError: position: "} +
                               error.what()};
    }
  }
};

typedef UTF8CodecBase<char> UTF8Codec;
} // namespace codec
} // namespace json

#endif
