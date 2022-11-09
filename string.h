#ifndef JSON_STRING_H
#define JSON_STRING_H

#include <string>

namespace json
{
    class String : public std::string
    {
    public:
        using std::string::string;
        String(std::string &str);
        String(std::string &&str);
        friend std::ostream &operator<<(std::ostream &out, const String &str);
    };
}

#endif