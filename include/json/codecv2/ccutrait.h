#ifndef JSON_CODECV2_CCUTRAIT_H
#define JSON_CODECV2_CCUTRAIT_H

#include <iostream>
#include <string>

namespace json {
namespace codecv2 {

template <typename CharT> class CCUTrait {
protected:
  using CharTraits = std::char_traits<CharT>;

public:
  using int_type = typename CharTraits::int_type;
  CCUTrait() : mData{std::char_traits<CharT>::eof()} {}
  CCUTrait(const int_type data) : mData{data} {}
  CCUTrait(const CharT ch) : mData{static_cast<int_type>(ch)} {}

  virtual bool isValid() const = 0;
  virtual std::size_t payloadSize() const = 0;
  virtual int_type payload() const = 0;

  friend std::ostream &operator<<(std::ostream &out, const CCUTrait &ccu) {
    out << ccu.mData;
    return out;
  }

protected:
  virtual ~CCUTrait() = default;
  const int_type mData{};
};

} // namespace codecv2
} // namespace json

#endif
