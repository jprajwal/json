#include "codec/rune.h"
#include "codec/utf16codec.h"
#include "test.h"
#include <cassert>

void testUTF16SimpleSurrogatePairEncode() {
  json::codec::UTF16Codec codec{};
  json::codec::Rune rune{0x10437};
  auto result{codec.encodeSingle(rune)};
  json::log << std::hex << "hs: " << result.at(0) << std::endl;
  json::log << std::hex << "ls: " << result.at(1) << std::endl;
  assert(result.at(0) == 0xD801);
  assert(result.at(1) == 0xDC37);
}

int main() { testUTF16SimpleSurrogatePairEncode(); }