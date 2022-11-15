#ifndef JSON_CODEC_LEADINGCODEUNIT_H
#define JSON_CODEC_LEADINGCODEUNIT_H

#include <cstdint>
#include <stdexcept>
#include <string>

namespace json {
namespace codec {

template <typename CharT, typename Traits> class LeadingCodeUnitBase {
public:
  LeadingCodeUnitBase() : mData{Traits::eof()} {}
  LeadingCodeUnitBase(const typename Traits::int_type data) : mData{data} {}
  LeadingCodeUnitBase(const CharT ch)
      : mData{static_cast<typename Traits::int_type>(ch)} {}

  std::size_t continuationUnitsCount() {
    throw std::runtime_error{"Operation not implemented"};
  }
  bool isValid() { throw std::runtime_error{"Operation not implemented"}; }
  typename Traits::int_type payload() {
    throw std::runtime_error{"Operation not implemented"};
  }
  std::size_t payloadSize() {
    throw std::runtime_error{"Operation not implemented"};
  }

protected:
  const typename Traits::int_type mData{};
};

template <typename CharT, typename Traits = std::char_traits<CharT>>
class LeadingCodeUnit : public LeadingCodeUnitBase<CharT, Traits> {};

} // namespace codec
} // namespace json

#endif