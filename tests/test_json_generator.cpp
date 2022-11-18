#include "codec/jsoncodec.h"
#include "codec/utf16/utf16char.h"
#include "codec/utf16codec.h"
#include "codec/utf8codec.h"
#include "json.h"
#include "test.h"

#include <cstdint>
#include <sstream>
#include <string>

/*
std::string testJSONGenerator(std::string &str) {
  json::codec::UTF8Codec utf8{};
  json::codec::UTF16Codec utf16{};
  std::string result{};
  auto runeArray{utf8.decode(str)};
  result.push_back('\"');
  for (auto rune : runeArray) {
    json::log << std::hex << "rune: " << rune << std::endl;
    if (!rune.isValid()) {
      json::log << "rune is invalid" << std::endl;
      return result;
    }
    if (rune.isASCII()) {
      result.push_back(static_cast<char>(rune));
    } else {
      json::codec::UTF16Char ch{utf16.encodeSingle(rune)};
      for (std::size_t i{0}; i < ch.length(); ++i) {
        std::stringstream stream;
        result.push_back('\\');
        result.push_back('u');
        stream << std::hex << static_cast<std::uint16_t>(ch.at(i));
        result += stream.str();
      }
    }
  }
  result.push_back('\"');
  return result;
}
*/

int main() {
  json::Json js{"こんにabcち𝄞は"};
  // auto result{testJSONGenerator(str)};
  std::stringstream ss;
  json::JSONCodec codec{};
  codec.encode(ss, js);
  json::log << ss.str() << std::endl;
}
