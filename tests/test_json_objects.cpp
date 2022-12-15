
#include "json.h"
#include "test.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

void testCstr() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  assert(j.type() == json::Type::object);
  json::log << j << std::endl;
}

void testKeys() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto keys = j.keys();
  json::print_list(keys);
}

void testValues() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto values = j.values();
  json::print_list(values);
}

void testItems() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto items = j.items();
  json::print_list(items);
}

void testIndexOperator() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  for (const auto &item : j.items()) {
    assert(item.second.copyString() == j[item.first].copyString());
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

void testInsert() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  j.update({{"key3", "value3"}, {"key4", "value4"}});
  json::log << j << std::endl;
  assert(j["key3"].copyString() == "value3");
  assert(j["key4"].copyString() == "value4");
}

void testInsert2() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  std::string k3 = "key3";
  std::string v3 = "value3";
  std::string k4 = "key4";
  std::string v4 = "value4";
  j.update({{k3, v3}, {k4, v4}});
  json::log << j << std::endl;
  assert(j["key3"].copyString() == "value3");
  assert(j["key4"].copyString() == "value4");
}

void testInsert3() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  std::vector<json::Json::object_t::value_type> pairs;
  for (int i = 3; i < 8; ++i) {
    std::string key = "key" + std::to_string(i);
    std::string value = "value" + std::to_string(i);
    pairs.push_back({key, value});
  }
  j.update(pairs);
  json::log << j << std::endl;
  assert(j["key3"].copyString() == "value3");
  assert(j["key4"].copyString() == "value4");
}

void testUsage() {
  using json::Json;
  json::log << "test..." << std::endl;
  Json obj = {{"key1", "value1"}, {"key2", "value2"}};
  json::log << obj << std::endl;
  Json str = obj.pop("key1");
  json::log << str << std::endl;
  json::log << obj["key2"] << std::endl;

  obj.update({{"key3", "value3"}, {"key4", "value4"}});
  json::log << obj << std::endl;

  Json key4 = obj.pop("key4");
  key4 = {{"key41", "value41"}, {"key42", "value42"}};
  json::log << key4 << std::endl;

  obj.set({"key4", key4});
  json::log << obj << std::endl;

  key4 = obj.pop("key4");
  key4.set({"key43", "value43"});
  obj.set({"key4", std::move(key4)});
  json::log << obj << std::endl;
}

void testTypeCastingByCopy() {
  using json::Json;
  Json obj = {{"key1", "value1"}, {"key2", "value2"}};
  Json::object_t map = obj;
  auto keys = obj.keys();
  assert(std::find(keys.cbegin(), keys.cend(), "key1") != keys.cend());
}

void testTypeCastingByMove() {
  using json::Json;
  Json obj = {{"key1", "value1"}, {"key2", "value2"}};
  Json::object_t map = std::move(obj);
  auto keys = obj.keys();
  assert(std::find(keys.cbegin(), keys.cend(), "key1") == keys.cend());
}

int main() {
  testCstr();
  testKeys();
  testValues();
  testItems();
  testIndexOperator();
  testIndexOperatorKeyError();
  testNestedObjectCreate();
  testInsert();
  testInsert2();
  testInsert3();
  testUsage();
  testTypeCastingByCopy();
  testTypeCastingByMove();
}
