#ifndef JSON_CODEC_CONTINUATIONCODEUNIT_H
#define JSON_CODEC_CONTINUATIONCODEUNIT_H

#include <stdexcept>
#include <string>

namespace json {
namespace codec {

template <typename CharT, typename Traits> class ContinuationCodeUnitBase {
public:
  ContinuationCodeUnitBase() : mData{Traits::eof()} {}
  ContinuationCodeUnitBase(typename Traits::int_type data) : mData{data} {}
  ContinuationCodeUnitBase(CharT ch)
      : mData{static_cast<typename Traits::int_type>(ch)} {}

  bool isValid() { throw std::runtime_error{"Operation not implemented"}; }
  std::size_t payloadSize() {
    throw std::runtime_error{"Operation not implemented"};
  }
  typename Traits::int_type payload() {
    throw std::runtime_error{"Operation not implemented"};
  }

protected:
  typename Traits::int_type mData{};
};

template <typename CharT, typename Traits = std::char_traits<CharT>>
class ContinuationCodeUnit : public ContinuationCodeUnitBase<CharT, Traits> {};

} // namespace codec
} // namespace json

#endif