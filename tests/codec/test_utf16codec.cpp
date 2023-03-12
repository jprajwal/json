#include "json/codec/rune.h"
#include "json/codec/utf16codec.h"
#include "json/test.h"
#include <cassert>

namespace utf16 = json::codec::utf16;

void testUTF16SimpleSurrogatePairEncode() {
  json::codec::Rune rune{0x10437};
  auto result{utf16::encodeOne(rune)};
  json::log << std::hex << "hs: " << result.at(0) << std::endl;
  json::log << std::hex << "ls: " << result.at(1) << std::endl;
  assert(result.at(0) == 0xD801);
  assert(result.at(1) == 0xDC37);
}

int main() { testUTF16SimpleSurrogatePairEncode(); }
