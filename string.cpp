#include "string.h"

#include <iostream>

namespace json
{

    String::String(std::string &str) : std::string{str} {}
    String::String(std::string &&str) : std::string{std::forward<std::string>(str)}
    {
    }

    std::ostream &operator<<(std::ostream &out, const String &str)
    {
        out << "\"" << static_cast<const std::string &>(str) << "\"";
        return out;
    }
}