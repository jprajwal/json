#ifndef JSON_JSON_DECODER_H
#define JSON_JSON_DECODER_H

#include "codecv2/decoder.h"
#include "codecv2/encoder.h"
#include "json.h"
#include "json_decode_error.h"
#include "json_text_iter.h"
#include "test.h"

#include <string>
#include <string_view>
#include <utility>

namespace json {
class JsonDecoder {
public:
  JsonDecoder() = default;
  JsonDecoder(std::string_view str) : m_str{str}, m_iter{str} {}

private:
  std::pair<bool, std::uint16_t> hexFromUnicodeText() {
    if (m_iter.peek_n(0, 6).starts_with("\\u")) {
      try {
        auto n = std::stoi(m_iter.take_n(6).substr(2), nullptr, 16);
        return {true, n};
      } catch (std::invalid_argument &exc) {
        json::log << "hexFromUnicodeText: Error: " << exc.what() << std::endl;
        return {false, 0};
      } catch (std::out_of_range &exc) {
        json::log << "hexFromUnicodeText: Error: " << exc.what() << std::endl;
        return {false, 0};
      }
    }
    return {false, 0};
  }

  std::string utf8FromUnicodeText() {
    using json::codec::Rune;
    namespace cvt = json::codecv2;
    cvt::UTF8Encoder utf8Encoder;
    cvt::UTF16Decoder utf16Decoder;

    std::string result;
    if (auto [success, hex] = hexFromUnicodeText(); success) {
      Rune rune(hex);
      if (rune.isBMP()) {
        result.append(utf8Encoder.encode({rune}));
      } else if (auto [success, sechex] = hexFromUnicodeText(); success) {
        std::basic_string<char16_t> toDecode;
        toDecode.push_back(static_cast<char16_t>(hex));
        toDecode.push_back(static_cast<char16_t>(sechex));
        auto runes = utf16Decoder.decode(toDecode);
        result.append(utf8Encoder.encode(runes));
      }
    }
    return result;
  }

  bool isDigit() {
    auto ch = m_iter.peek(0);
    return (ch >= 0x30 && ch <= 0x39);
  }

public:
  Json parseJsonString() {
    std::string result;
    // Consume the starting double quotes
    m_iter.take_n(1);

    while (m_iter.remaining() >= 0) {

      if (m_iter.remaining() == 0) {
        if (*m_iter != '"') {
          throw JsonDecodeError(m_iter.lineno(), m_iter.column(),
                                "missing closing double quotes");
        }
        ++m_iter;
        continue;
      }

      if (m_iter.peek(0) == '\\') {
        switch (m_iter.peek(1)) {
        case 'n':
          result.push_back('\n');
          m_iter.take_n(2);
          break;
        case 'r':
          result.push_back('\r');
          m_iter.take_n(2);
          break;
        case 't':
          result.push_back('\t');
          m_iter.take_n(2);
          break;
        case 'b':
          result.push_back('\b');
          m_iter.take_n(2);
          break;
        case 'f':
          result.push_back('\f');
          m_iter.take_n(2);
          break;
        case '"':
          result.push_back('"');
          m_iter.take_n(2);
          break;
        case 'u': {
          auto utf8 = utf8FromUnicodeText();
          if (utf8.length() == 0) {
            throw JsonDecodeError(m_iter.lineno(), m_iter.column(),
                                  "invalid unicode escape sequence");
          }
          result.append(utf8);
          break;
        }
        default:
          throw JsonDecodeError(m_iter.lineno(), m_iter.column(),
                                "invalid escape sequence");
        }
      } else {
        result.push_back(*m_iter++);
      }
    }
    return result;
  }

  Json parseJsonNumber() {
    if (!isDigit() && m_iter.peek(0) != '-') {
      throw JsonDecodeError(m_iter.lineno(), m_iter.column(), "not a number");
    }

    auto pos = m_str.substr(m_iter.index(), m_iter.remaining() + 1)
                   .find_first_not_of("0123456789.eE-+");
    json::log << "pos = " << pos << std::endl;
    if (pos == std::string_view::npos) {
      pos = m_str.length() - 1;
    }
    auto number = m_iter.take_n(pos + 1);
    json::log << "number = " << number << std::endl;
    try {
      if (number.find('.') == std::string::npos) {
        auto parsedNumber = std::stoi(number);
        return Json(Json::int_t{parsedNumber});
      } else {
        auto parsedNumber = std::stod(number);
        return Json(parsedNumber);
      }
    } catch (std::invalid_argument &exc) {
      json::log << "parJsonNumber: " << exc.what() << std::endl;
      throw JsonDecodeError(m_iter.lineno(), m_iter.column(), exc.what());
    } catch (std::out_of_range &exc) {
      json::log << "parsJsonNumber: " << exc.what() << std::endl;
      throw JsonDecodeError(m_iter.lineno(), m_iter.column(), exc.what());
    }
  }

private:
  std::string_view m_str{};
  JsonTextIter m_iter{};
};
} // namespace json

#endif
