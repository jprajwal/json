#ifndef JSON_JSONCOMMON_H
#define JSON_JSONCOMMON_H

#include "codec/utf16char.h"
#include "codec/utf16codec.h"
#include "codec/utf8codec.h"
#include "json.h"

#include <ios>
#include <iostream>
#include <sstream>

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

std::string Json::dumps() {
  std::stringstream result;

  switch (m_variant.type()) {

  case Type::string: {
    namespace utf8 = codec::utf8;
    namespace utf16 = codec::utf16;
    auto runeArray = utf8::decode(this->toString());
    result << '"';
    std::size_t index = 0;
    for (auto rune : runeArray) {
      // log << std::hex << "rune: " << rune << std::endl;
      if (!rune.isValid()) {
        // log << "rune is invalid" << std::endl;
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
  case Type::object:

    break;
  case Type::integer:
    result << toInteger();
    return result.str();
  case Type::floating_point:
    break;
  case Type::boolean:
    break;
  case Type::null:
    break;
  default:
    break;
  }
  return result.str();
}
} // namespace json

#endif
