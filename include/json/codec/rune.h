#ifndef JSON_CODEC_RUNE_H
#define JSON_CODEC_RUNE_H

#include <cstdint>
#include <iostream>

namespace json {
namespace codec {

class Rune {
  static constexpr uint32_t BMP_1_START = 0x0000;
  static constexpr uint32_t BMP_1_END = 0xD7FF;
  static constexpr uint32_t BMP_2_START = 0xE000;
  static constexpr uint32_t BMP_2_END = 0xFFFF;
  static constexpr uint32_t SMP_START = 0x10000;
  static constexpr uint32_t SMP_END = 0x10FFFF;

  static constexpr uint32_t ASCII_START = 0x0000;
  static constexpr uint32_t ASCII_END = 0x007F;
  static constexpr uint32_t CTRL_CHRS_START = 0x0000;
  static constexpr uint32_t CTRL_CHRS_END = 0x001F;
  static constexpr uint32_t DIGIT_START = 0x0030;
  static constexpr uint32_t DIGIT_END = 0x0039;

public:
  Rune() : mData{} {}
  Rune(std::uint32_t data) : mData{data} {}
  constexpr bool isValid() const { return (isASCII() || isBMP() || isSMP()); }
  constexpr bool isASCII() const {
    return (ASCII_START <= mData) && (mData <= ASCII_END);
  }
  constexpr bool isDigit() const {
    return (DIGIT_START <= mData) && (mData <= DIGIT_END);
  }
  constexpr bool isControlChar() const {
    return ((CTRL_CHRS_START <= mData) && (mData <= CTRL_CHRS_END)) ||
           mData == ASCII_END;
  }
  constexpr bool isBMP() const {
    return (((BMP_1_START <= mData) && (mData <= BMP_1_END)) ||
            ((BMP_2_START <= mData) && (mData <= BMP_2_END)));
  }
  constexpr bool isSMP() const {
    return (SMP_START <= mData) && (mData <= SMP_END);
  }
  Rune operator<<(std::size_t npos) { return mData << npos; }
#if 0
  friend Rune operator|(Rune lhs, Rune rhs) { return lhs.mData | rhs.mData; }
  friend Rune operator|(Rune lhs, std::uint32_t rhs) { return lhs.mData | rhs; }
  friend Rune operator|(std::uint32_t lhs, Rune rhs) { return lhs | rhs.mData; }
#else
  Rune operator|(Rune other) const { return mData | other.mData; }
  Rune operator|(std::uint32_t other) const { return mData | other; }
  operator std::uint32_t() { return mData; }
#endif
  friend std::ostream &operator<<(std::ostream &out, const Rune &rune) {
    out << rune.mData;
    return out;
  }

  bool operator==(const Rune other) const { return mData == other.mData; }
  bool operator==(const std::uint32_t other) const { return mData == other; }
  bool operator!=(const Rune other) const { return !(*this == other); }
  operator char() const { return static_cast<char>(mData); }
  operator std::uint32_t() const { return static_cast<uint32_t>(mData); }
  operator std::uint16_t() const { return static_cast<uint16_t>(mData); }

private:
  std::uint32_t mData{};
};

} // namespace codec
} // namespace json

#endif
