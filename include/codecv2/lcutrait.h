#ifndef JSON_CODECV2_LCUTRAIT_H
#define JSON_CODECV2_LCUTRAIT_H

#include <string>

namespace json {
namespace codecv2 {

template <typename CharT> class LCUTrait {
protected:
  using CharTraits = std::char_traits<CharT>;

public:
  using int_type = typename CharTraits::int_type;
  LCUTrait() : mData{std::char_traits<CharT>::eof()} {}
  LCUTrait(const int_type data) : mData{data} {}
  LCUTrait(const CharT ch) : mData{static_cast<int_type>(ch)} {}

  virtual std::size_t continuationUnitCount() const = 0;
  virtual bool isValid() const = 0;
  virtual std::size_t payloadSize() const = 0;
  virtual int_type payload() const = 0;

protected:
  virtual ~LCUTrait() = default;
  const int_type mData{};
};

} // namespace codecv2
} // namespace json

#endif
