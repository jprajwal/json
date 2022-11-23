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

#include "json.h"
#include "test.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void testDefaultCstr() {
  json::Json j;
  assert(j.type() == json::Type::Null);
}

void testJsonStrCopyCstr() {
  std::string s{"test"};
  json::Json j{s};
  assert(j.type() == json::Type::String);
}

void testJsonStrMoveCstr() {
  json::Json j{"test"};
  assert(j.type() == json::Type::String);
}

void testJsonConstStringIterator() {
  json::Json j{"test"};
  for (auto ch{j.stringCBegin()}; ch != j.stringCEnd(); ++ch) {
    json::log << *ch << ' ';
  }
  json::log << std::endl;
  assert(j == "test");
}

void testJsonStringOstreamInsertion() {
  json::Json j{"test"};
  std::stringstream ss;
  ss << j;
  json::log << ss.str();
  assert(ss.str() == "test");
}

int main() {
  testDefaultCstr();
  testJsonStrCopyCstr();
  testJsonStrMoveCstr();
  testJsonConstStringIterator();
  testJsonStringOstreamInsertion();
}
