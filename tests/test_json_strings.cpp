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
  const json::Json j1{"test"};
  json::str_iter<json::Json> iter(j1);
  for (const auto ch : iter) {
    json::log << ch << ' ';
  }
  json::log << std::endl;
  assert(j1 == "test");
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
