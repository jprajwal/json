#ifndef JSON_JSONCOMMON_H
#define JSON_JSONCOMMON_H

#include "json/codec/rune.h"
#include "json/codec/utf16codec.h"
#include "json/codec/utf8codec.h"
#include "json/codecv2/decoder.h"
#include "json/codecv2/encoder.h"
#include "json/codecv2/utf16_lcu.h"
#include "json/codecv2/utf8_lcu.h"
#include "json/json.h"
#include "json/json_decode_error.h"
#include "json/json_decoder.h"
#include "json/json_text_iter.h"

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
  case Type::floating_point:
    out << jsn.m_variant.floating_point();
    break;
  case Type::boolean:
    out << std::boolalpha;
    out << jsn.m_variant.boolean();
    out << std::noboolalpha;
    break;
  case Type::list:
    out << jsn.m_variant.list();
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
  case Type::list:
    return m_variant.list() == other.m_variant.list();
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
        result << ", ";
      }
      result << Json{key}.dumps() << ": " << value.dumps();
      ++count;
    }
    result << '}';
    return result.str();
  }
  case Type::list: {
    result << '[';
    std::size_t count = 0;
    for (const auto &item : list_iter()) {
      if (count > 0) {
        result << ", ";
      }
      result << item.dumps();
      ++count;
    }
    result << ']';
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

Json Json::loads(const std::string &s) {
  json::codecv2::UTF8Decoder().decode(s);

  JsonDecoder jsonDecoder(std::string_view{s});

  return jsonDecoder.decode();
}
} // namespace json

#endif
