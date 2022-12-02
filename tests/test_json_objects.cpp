
#include "json.h"
#include "test.h"

#include <cassert>
#include <iostream>

void testJsonObjectCstr() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  assert(j.type() == json::Type::Object);
  json::log << j << std::endl;
}

void testJsonObjectfind() {
  json::Json j{{"key1", "value1"}, {"key2", "value2"}};
  auto result = j.objFind("key1");
  json::obj_iter<json::Json> iter{j};
  assert(result != iter.end());
  assert(result->second == "value1");
}

void testJsonObjectInsert() {
  json::Json js{{"key1", "value1"}, {"key2", "value2"}};
  json::obj_iter<json::Json> iter{js};
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
  json::obj_iter<json::Json> iter{js};
  js.objErase("key1");
  js.objErase("key3");
  json::log << js << std::endl;
  assert(js.objFind("key1") == iter.end());
  assert(js.objFind("key3") == iter.end());
}

void testJsonNestedObjectCreate() {
  json::Json js{{"k1", "v1"}, {"k2", {{"k21", "v21"}, {"k22", "v22"}}}};
  json::obj_iter iter{js};
  json::log << js << std::endl;
  json::log << js["k2"]["k22"] << std::endl;
}

int main() {
  testJsonObjectCstr();
  testJsonObjectfind();
  testJsonObjectInsert();
  testJsonObjectErase();
  testJsonNestedObjectCreate();
}
