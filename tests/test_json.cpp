/*
#include "json.h"

#include <cassert>

void testJsonObjectValidIndexing() {
  using json::Json;
  Json j{{"key1", "value1"}, {"key2", "value2"}};
  Json j2{{"key3", j}};
  Json j3{{"key4", j2}, {"key5", Json{}}};
  assert(j["key1"] == "value1");
}

void testObjectInvalidIndexing() {
  using json::Json;
  Json j{"string"};
  try {
    j["something"];
  } catch (std::runtime_error &error) {
    assert(error.what() ==
           std::string{"string indexing not supported for this json type"});
  }
}

void testObjectValidIndexButNotPresent() {
  using json::Json;
  Json j{{"key1", "value1"}, {"key2", "value2"}};
  try {
    j["not_existing_key"];
    assert(true);
  } catch (std::runtime_error &error) {
    assert(false);
  }
}

int main() {
  testJsonObjectValidIndexing();
  testObjectInvalidIndexing();
  testObjectValidIndexButNotPresent();
}
*/

int main() {}