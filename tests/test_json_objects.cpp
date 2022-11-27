
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
  auto result = j.obj_find("key1");
  json::obj_iter<json::Json> iter{j};
  assert(result != iter.end());
  assert(result->second == "value1");
}

int main() { testJsonObjectCstr(); }
