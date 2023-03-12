#ifndef JSON_CODEC_ICHAR_H
#define JSON_CODEC_ICHAR_H

#include "json/test.h"

#include <stdexcept>

namespace json {
namespace codec {

template <typename CharT> class ICharBase {
public:
  ICharBase() : mLength{0}, mCodeUnits{} {}
  ICharBase(std::size_t len) : mLength{len}, mCodeUnits{new CharT[len]{}} {}
  ICharBase(std::size_t len, const CharT *chptr)
      : mLength{len}, mCodeUnits{new CharT[len]{}} {
    for (std::size_t index{0}; index < len; ++index) {
      mCodeUnits[index] = chptr[index];
    }
  }
  ICharBase(ICharBase<CharT> &other) : mLength{}, mCodeUnits{} {
    if (other.mLength == 0) {
      mLength = 0;
      mCodeUnits = nullptr;
      return;
    }
    mLength = other.mLength;
    mCodeUnits = new CharT[mLength]{};
    for (std::size_t index{0}; index < mLength; ++index) {
      mCodeUnits[index] = other.mCodeUnits[index];
    }
  }
  ICharBase(ICharBase<CharT> &&other) noexcept : mLength{}, mCodeUnits{} {
    if (other.mLength == 0) {
      mLength = 0;
      mCodeUnits = nullptr;
      return;
    }
    mLength = other.mLength;
    mCodeUnits = other.mCodeUnits;
    other.mLength = 0;
    other.mCodeUnits = nullptr;
  }
  ICharBase<CharT> &operator=(const ICharBase<CharT> &other) {
    json::log << "ICharBase copy constructor" << std::endl;
    if (this == &other) {
      return *this;
    }
    if (other.mLength == 0) {
      mLength = 0;
      mCodeUnits = nullptr;
      return *this;
    }
    mLength = other.mLength;
    mCodeUnits = new CharT[mLength]{};
    for (std::size_t index{0}; index < mLength; ++index) {
      mCodeUnits[index] = other.mCodeUnits[index];
    }
    return *this;
  }
  ICharBase<CharT> &operator=(ICharBase<CharT> &&other) noexcept {
    json::log << "ICharBase move constructor" << std::endl;
    if (other.mLength == 0) {
      mLength = 0;
      mCodeUnits = nullptr;
      return *this;
    }
    mLength = other.mLength;
    mCodeUnits = other.mCodeUnits;
    other.mLength = 0;
    other.mCodeUnits = nullptr;
    return *this;
  }
  ~ICharBase() {
    if (mCodeUnits) {
      delete[] mCodeUnits;
    }
  }
  CharT &at(std::size_t index) {
    if (index >= mLength) {
      throw std::runtime_error{"Index out of range"};
    }
    return mCodeUnits[index];
  }

  std::size_t length() const { return mLength; }

private:
  std::size_t mLength{};
  CharT *mCodeUnits{};
};

} // namespace codec
} // namespace json

#endif