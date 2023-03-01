#ifndef JSON_JSONCOMMON_H
#define JSON_JSONCOMMON_H

#include "codec/rune.h"
#include "codec/utf16codec.h"
#include "codec/utf8codec.h"
#include "codecv2/codec.h"
#include "codecv2/encoder.h"
#include "codecv2/utf16_lcu.h"
#include "codecv2/utf8_lcu.h"
#include "json.h"
#include "test.h"

#include <bitset>
#include <cstdint>
#include <cstring>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace json {
std::ostream &operator<<(std::ostream &out, const Json &jsn) {
  switch (jsn.m_variant.type()) {
  case Type::string:
    out << jsn.m_variant.str();
    break;
  case Type::null:
    out << jsn.m_variant.null();
    break;
  case Type::object:
    out << jsn.m_variant.object();
    break;
  case Type::integer:
    out << jsn.m_variant.integer();
    break;
  case Type::boolean:
    out << std::boolalpha;
    out << jsn.m_variant.boolean();
    out << std::noboolalpha;
    break;
  default:
    out << "Not Implemented";
  }
  return out;
}

bool Json::operator==(const Json &other) const {
  if (m_variant.type() != other.type()) {
    return false;
  }

  switch (other.m_variant.type()) {
  case Type::string:
    return m_variant.str() == other.m_variant.str();
  case Type::object:
    return m_variant.object() == other.m_variant.object();
  case Type::integer:
    return m_variant.integer() == other.m_variant.integer();
  case Type::floating_point:
    return m_variant.floating_point() == other.m_variant.floating_point();
  case Type::boolean:
    return m_variant.boolean() == other.m_variant.boolean();
  case Type::null:
    return true;
  default:
    return false;
  }
}

bool Json::operator!=(const Json &other) const { return !(*this == other); }

std::string Json::dumps() const {
  std::stringstream result;

  switch (m_variant.type()) {

  case Type::string: {
    namespace utf8 = codec::utf8;
    namespace utf16 = codec::utf16;
    auto runeArray = utf8::decode(this->toString());
    result << '"';
    std::size_t index = 0;
    for (auto rune : runeArray) {
      if (!rune.isValid()) {
        throw std::runtime_error{std::string{"JsonEncodeError: position: "} +
                                 std::to_string(index)};
      }
      if (rune.isASCII()) {
        auto chr = static_cast<char>(rune);
        if (chr == '\n') {
          result << '\\' << 'n';
        } else if (chr == '\r') {
          result << '\\' << 'r';
        } else if (chr == '\t') {
          result << '\\' << 't';
        } else if (chr == '"') {
          result << '\\' << '"';
        } else if (chr == '\\') {
          result << "\\\\";
        } else if (chr == '\b') {
          result << '\\' << 'b';
        } else if (chr == '\f') {
          result << '\\' << 'f';
        } else if (rune.isControlChar()) {
          result << '\\' << 'u';
          result << std::hex << static_cast<std::uint16_t>(rune);
        } else {
          result << chr;
        }
      } else {
        codec::UTF16Char ch{utf16::encodeOne(rune)};
        for (std::size_t i = 0; i < ch.length(); ++i) {
          std::stringstream stream;
          result << '\\';
          result << 'u';
          stream << std::hex << static_cast<std::uint16_t>(ch.at(i));
          result << stream.str();
        }
      }
      ++index;
    }
    result << '"';
    return result.str();
  }
  case Type::object: {
    result << '{';
    std::size_t count = 0;
    for (auto &[key, value] : items()) {
      if (count > 0) {
        result << ",";
      }
      result << Json{key}.dumps() << ":" << value.dumps();
      ++count;
    }
    result << '}';
    return result.str();
  }
  case Type::integer:
    result << toInteger();
    return result.str();
  case Type::floating_point:
    result << toFloatingPoint();
    return result.str();
  case Type::boolean:
    result << std::boolalpha << toBoolean();
    return result.str();
  case Type::null:
    result << toNull();
    return result.str();
  default:
    return result.str();
  }
}

static std::string parseJsonString(const std::string &str) {
  std::string result;
  for (std::size_t i = 1; i < str.size() - 1; ++i) {
    auto ch = str[i];
    if (i == str.size() - 1 && ch != '"') {
      throw -1;
    }
    if (ch == '\\') {
      if (i == str.size() - 1) {
        throw -1;
      }
      auto nch = str[i + 1];
      if (nch == 'n') {
        result.push_back('\n');
        ++i;
      } else if (nch == 'r') {
        result.push_back('\r');
        ++i;
      } else if (nch == 't') {
        result.push_back('\t');
        ++i;
      } else if (nch == 'b') {
        result.push_back('\b');
        ++i;
      } else if (nch == 'f') {
        result.push_back('\f');
        ++i;
      } else if (nch == '"') {
        result.push_back('"');
        ++i;
      } else if (nch == 'u') {
        std::size_t j = i + 2;
        auto first = std::stoi(str.substr(j, 4), nullptr, 16);
        json::log << "first: " << std::bitset<16>(first) << ", " << std::hex
                  << first << std::endl;
        j += 4;
        json::codecv2::UTF8Encoder encoder;
        json::codecv2::UTF16Decoder decoder;
        json::codec::Rune rune(first);
        json::log << std::boolalpha << "is rune valid: " << rune.isValid()
                  << std::endl;

        if (rune.isBMP()) {
          auto tmp = encoder.encode({rune});
          auto byteArray = tmp.c_str();
          auto len = std::strlen(byteArray);
          for (std::size_t i = 0; i < len; i++) {
            json::log << std::bitset<8>(byteArray[i]) << ' ';
          }
          json::log << std::endl;
          result.append(tmp);
        } else {
          j += 2;
          auto second = std::stoi(str.substr(j, 4), nullptr, 16);
          j += 4;
          std::basic_string<char16_t> toDecode;
          toDecode.push_back(static_cast<char16_t>(first));
          toDecode.push_back(static_cast<char16_t>(second));
          auto runes = decoder.decode(toDecode);
          result.append(encoder.encode(runes));
        }
        i = j - 1;
        continue;
      } else {
        throw -1;
      }
    } else {
      result.push_back(ch);
    }
  }
  return result;
}

Json Json::loads(const std::string &s) {
  if (s.size() == 0) {
    // TODO: Figure out exception type
    throw -1;
  }
  // step 1: Check encoding is utf-8 or not.
  auto runes = json::codecv2::UTF8Decoder().decode(s);

  // step 2: verify structure is according to json grammar.
  // Identify outermost element.
  auto rune = runes[0];
  if (!rune.isASCII()) {
    throw -1;
  }

  Type outermostType = Type::null;
  auto ch = static_cast<char>(rune);
  if (ch == '{') {
    outermostType = Type::object;
  } else if (ch == '"') {
    outermostType = Type::string;
  } else if (rune.isDigit()) {
    outermostType = Type::floating_point;
  } else if (s == "true" || s == "false") {
    outermostType = Type::boolean;
  } else if (s == "null") {
    outermostType = Type::null;
  } else {
    throw -1;
  }

  switch (outermostType) {
  case Type::string:
    return parseJsonString(s);
  default:
    return Json();
  }
}
} // namespace json

#endif
