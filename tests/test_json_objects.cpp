
#include "json.h"
#include "test.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

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

int main() {
  testCstr();
  testKeys();
  testValues();
  testItems();
  testIndexOperator();
  testIndexOperatorKeyError();
  testNestedObjectCreate();
}
