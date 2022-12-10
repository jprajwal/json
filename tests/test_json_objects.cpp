
#include "json.h"
#include "test.h"

#include <cassert>
#include <iostream>

void testJsonObjectCstr() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  assert(j.type() == json::Type::object);
  json::log << j << std::endl;
}

void testJsonObjectKeys() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto keys = j.keys();
  json::print_list(keys);
}

void testJsonObjectValues() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto values = j.values();
  json::print_list(values);
}

void testJsonObjectItems() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto items = j.items();
  json::print_list(items);
}
/*
void testJsonObjectfind() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto result = j.objFind("key1");
  json::obj_iter<> iter{j};
  assert(result != iter.end());
  assert(result->second == "value1");
}

void testJsonObjectInsert() {
  json::Json js{{"key1", "value1"}, {"key2", "value2"}};
  json::obj_iter<> iter{js};
  if (js.objFind("key1") == iter.end()) {
    js["key1"] = "new_value1";
  }
  if (js.objFind("key3") == iter.end()) {
    js["key3"] = "value3";
  }
  json::log << js << std::endl;
  assert(js["key1"] == "value1");
  assert(js["key3"] == "value3");
}

void testJsonObjectErase() {
  json::Json js{{"key1", "value1"}, {"key2", "value2"}};
  js.objErase("key1");
  js.objErase("key3");
  json::log << js << std::endl;
  json::obj_iter<> iter{js};
  assert(js.objFind("key1") == iter.end());
  assert(js.objFind("key3") == iter.end());
}

void testJsonNestedObjectCreate() {
  json::Json js{{"k1", "v1"}, {"k2", {{"k21", "v21"}, {"k22", "v22"}}}};
  json::log << js << std::endl;
  json::log << js["k2"]["k22"] << std::endl;
}

void testJsonObjectIterationOnMutatedObject() {
  json::Json js{{"key1", "value1"}, {"key2", "value2"}};
  json::obj_iter<> iter{js};
  js = json::Json{"hello"};
  for (auto &item : iter) {
    json::log << item << ' ';
  }
}
*/

int main() {
  testJsonObjectCstr();
  testJsonObjectKeys();
  testJsonObjectValues();
  testJsonObjectItems();
  /*
testJsonObjectfind();
testJsonObjectInsert();
testJsonObjectErase();
testJsonNestedObjectCreate();
testJsonObjectIterationOnMutatedObject();
  */
}
