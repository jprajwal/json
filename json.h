#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "string.h"
#include "object.h"
#include "null.h"

#include <iostream>
#include <memory>
#include <initializer_list>

namespace json
{
    class Object;
    class Json;
    using PairOfStrings = std::pair<String, String>;
    using PairOfStringAndJson = std::pair<String, Json>;

    class Json
    {

    public:
        // default json object
        Json();

        // json string
        Json(std::string str);
        Json(const char *str);

        // json object
        Json(std::initializer_list<PairOfStringAndJson> pairs);

        // TODO: copy and move constructors required?
        Json(const Json &);
        Json(const Json &&);

        // TODO: serializer? or seperate interface needed?
        friend std::ostream &operator<<(std::ostream &out, const Json &js);

        // object related operations
        // 1. object access interface
        Json &operator[](const String &key);

        enum class Type
        {
            Object,
            String,
            Null,
        };

    private:
        Type mType{Type::Null};
        Null mNull{};
        std::shared_ptr<Object> mObjectPtr{};
        String mString{};

    private:
        Json &operator=(const Json &);
        Json &operator=(const Json &&);
    };

}

#endif