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
        Json();
        Json(std::string str);
        Json(const char *str);
        Json(std::initializer_list<PairOfStringAndJson> pairs);
        Json(const Json &);
        Json(const Json &&);

        friend std::ostream &operator<<(std::ostream &out, const Json &js);

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