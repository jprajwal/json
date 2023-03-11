
#include "json.h"
#include "test.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

void testCstr() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  json::Json j2{j};
  assert(j.type() == json::Type::object);
  assert(j2.type() == json::Type::object);
}

void testKeys() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto keys = j.keys();
  json::print_list(keys);
  assert(json::is_same_list(
      keys, std::vector<json::Json::object_t::key_type>{"key1", "key2"}));
}

void testValues() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto values = j.values();
  json::print_list(values);
  assert(json::is_same_list(
      values,
      std::vector<json::Json::object_t::mapped_type>{"value1", "value2"}));
}

void testItems() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto items = j.items();
  json::print_list(items);
  assert(
      json::is_same_list(items, std::vector<json::Json::object_t::value_type>{
                                    {"key1", "value1"}, {"key2", "value2"}}));
}

void testIndexOperator() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  for (const auto &item : j.items()) {
    assert(item.second.toString() == j[item.first].toString());
  }
}

void testIndexOperatorKeyError() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  try {
    j["test_key"];
  } catch (std::runtime_error &error) {
    json::log << error.what() << std::endl;
    assert(std::string{error.what()} == "KeyError: test_key");
  }
}

void testNestedObjectCreate() {
  json::Json js{{"k1", "v1"}, {"k2", {{"k21", "v21"}, {"k22", "v22"}}}};
  json::log << js << std::endl;
  json::log << js["k2"]["k22"] << std::endl;
}

void testUpdate1() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  j.update({{"key3", "value3"}, {"key4", "value4"}});
  json::log << j << std::endl;
  assert(j["key3"].toString() == "value3");
  assert(j["key4"].toString() == "value4");
}

void testUpdate2() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  std::string k3 = "key3";
  std::string v3 = "value3";
  std::string k4 = "key4";
  std::string v4 = "value4";
  j.update({{k3, v3}, {k4, v4}});
  json::log << j << std::endl;
  assert(j["key3"].toString() == "value3");
  assert(j["key4"].toString() == "value4");
}

void testUpdate3() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  std::vector<json::Json::object_t::value_type> pairs;
  for (int i = 3; i < 8; ++i) {
    std::string key = "key" + std::to_string(i);
    std::string value = "value" + std::to_string(i);
    pairs.push_back({key, value});
  }
  j.update(pairs);
  json::log << j << std::endl;
  assert(j["key3"].toString() == "value3");
  assert(j["key4"].toString() == "value4");
}

void testUsage() {
  using json::Json;
  Json person = Json::object_t{};

  person["name"] = "John Doe";
  person["age"] = 27l;
  person["relations"] = Json::object_t();
  person["relations"]["father"] = "John Doe's father";
  person["relations"]["mother"] = "John Doe's mother";
  person["experience"] = {{"company1", "1 month"}, {"company2", "6 years"}};
  person["is_male"] = true;

  auto experience = person.pop("experience");
  experience.set({"company3", "0 years"});
  person.set({"experience", std::move(experience)});
  json::log << person << std::endl;
  assert(person["experience"]["company3"].toString() == "0 years");
}

void testTypeCastingByCopy() {
  using json::Json;
  Json obj = {{"key1", "value1"}, {"key2", "value2"}};
  Json::object_t map = obj;
  auto keys = obj.keys();
  assert(std::find(keys.begin(), keys.end(), "key1") != keys.end());
}

void testTypeCastingByMove() {
  using json::Json;
  Json obj = {{"key1", "value1"}, {"key2", "value2"}};
  Json::object_t map = std::move(obj);
  auto keys = obj.keys();
  assert(std::find(keys.begin(), keys.end(), "key1") == keys.end());
}

void testDumps() {
  using json::Json;
  Json person = Json::object_t{};

  person["name"] = "John Doe";
  person["age"] = 27l;
  person["relations"] = Json::object_t();
  person["relations"]["father"] = "John Doe's father";
  person["relations"]["mother"] = "John Doe's mother";
  person["experience"] = {{"company1", "1 month"}, {"company2", "6 years"}};
  person["is_male"] = true;
  json::log << "dumped object: " << person.dumps() << std::endl;
}

void testLoads() {

  using json::Json;
  std::string test =
      R"({
          "test_key": {
            "test_nested_key": "test_nested_value"
          }
        })";
  try {
    Json js = Json::loads(test);
    assert(js.type() == json::Type::object);
    auto map = js.toObject();
    // assert(map["test_key"] == "test_value");
    json::log << "loaded json = " << js << std::endl;
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    throw;
  }
}

void testLoadsDifferentDataTypes() {

  using json::Json;
  std::string test =
      R"({
          "test_key": {
            "test_nested_key": "test_nested_value",
            "k2": 1234,
            "k3": -1234,
            "k4": 1234.1234,
            "k5": true,
            "k6": false,
            "k7": null
          }
        })";
  try {
    Json js = Json::loads(test);
    json::log << "loaded json = " << js << std::endl;
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}
void testLoadsExtraComma() {

  using json::Json;
  std::string test =
      R"({
          "test_key": {
            "test_nested_key": "test_nested_value",
          }
        })";
  try {
    Json js = Json::loads(test);
    json::log << "loaded json = " << js << std::endl;
    assert(false);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(true);
  }
}

void testLoadsEmptyObject() {

  using json::Json;
  std::string test = R"({})";
  try {
    Json js = Json::loads(test);
    json::log << "loaded json = " << js << std::endl;
    assert(js.toObject().empty());
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

void testLoadsMissingClosingBraces() {

  using json::Json;
  std::string test =
      R"({
          "test_key": {
            "test_nested_key": "test_nested_value"
          }
        )";
  try {
    Json js = Json::loads(test);
    json::log << "loaded json = " << js << std::endl;
    assert(false);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(true);
  }
}

int main() {
  testCstr();
  testKeys();
  testValues();
  testItems();
  testIndexOperator();
  testIndexOperatorKeyError();
  testNestedObjectCreate();
  testUpdate1();
  testUpdate2();
  testUpdate3();
  testUsage();
  testTypeCastingByCopy();
  testTypeCastingByMove();
  testDumps();
  testLoads();
  testLoadsDifferentDataTypes();
  testLoadsExtraComma();
  testLoadsMissingClosingBraces();
  testLoadsEmptyObject();
}
