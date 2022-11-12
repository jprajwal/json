#ifndef JSON_CODEC_RUNE_H
#define JSON_CODEC_RUNE_H

#include <cstdint>

namespace json {
namespace codec {
using Rune = std::uint32_t;
namespace constants {
inline constexpr std::size_t sizeOfRune = sizeof(Rune);
}
/*
using UnicodeCodePoint = std::uint32_t;

template <typename CharT>
class Rune
{
public:
    Rune() = default;

    Char getCharacter() { return mChar; }
    UnicodeCodePoint getUnicodeCodePoint() { return mCodePoint; }

private:
    UnicodeCodePoint mCodePoint{};
    char mChar[sizeof UnicodeCodePoint];
};
*/
} // namespace codec

} // namespace json

#endif