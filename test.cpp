
#include "json.h"

#include <iostream>

void testJsonInitializerList()
{
    using json::Json;
    Json j{{"key1", Json{"value1"}}, {"key2", Json{"value2"}}};
    std::cout << j << std::endl;
}

void testJsonObjectComplexType()
{
    using json::Json;
    Json j{{"key1", Json{"value1"}}, {"key2", Json{"value2"}}};
    Json j2{{"key3", j}};
    Json j3{{"key4", j2}, {"key5", Json{}}};
    std::cout << j3 << std::endl;
}

void testJsonCopyConstructor()
{
    std::cout << "testJsonCopyConstructor" << std::endl;
    json::Json j{"hello world"};
    json::Json jc{j};
}

void testJsonObjectCreation()
{
    using json::Json;
    Json j{{"key1", "value1"}, {"key2", "value2"}};
    std::cout << j << std::endl;
}

void testObjectValidIndexing()
{
    using json::Json;
    Json j{{"key1", "value1"}, {"key2", "value2"}};
    std::cout << "key: key1, value: " << j["key1"] << std::endl;
}

void testObjectInvalidIndexing()
{
    using json::Json;
    Json j{"string"};
    try
    {
        j["something"];
    }
    catch (std::runtime_error &error)
    {
        std::cout << error.what() << std::endl;
    }
}

void testObjectValidIndexButNotPresent()
{
    using json::Json;
    Json j{{"key1", "value1"}, {"key2", "value2"}};
    // std::cout << "key: key1, value: " << j["invalid"] << std::endl;
    try
    {
        const auto &value{j["not_existing_key"]};
    }
    catch (std::runtime_error &error)
    {
        std::cout << error.what() << std::endl;
    }
}

int main()
{
    testJsonInitializerList();
    testJsonCopyConstructor();
    testJsonObjectComplexType();
    testJsonObjectCreation();
    testObjectValidIndexing();
    testObjectValidIndexButNotPresent();
    testObjectInvalidIndexing();
}