#ifndef JSON_CODEC_ICODEC_H
#define JSON_CODEC_ICODEC_H

#include <vector>

namespace json {
namespace codec {
class ICodec {
public:
  virtual std::vector<char> encode(const std::vector<Rune> &) = 0;
  virtual std::vector<Rune> decode(const std::vector<char> &) = 0;
};
} // namespace codec
} // namespace json

#endif