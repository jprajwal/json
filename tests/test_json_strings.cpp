#include "json.h"
#include "test.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void testDefaultCstr() {
  json::Json j;
  assert(j.type() == json::Type::null);
}

void testJsonStrCopyCstr() {
  std::string s{"test"};
  json::Json j{s};
  assert(j.type() == json::Type::string);
}

void testJsonStrMoveCstr() {
  json::Json j{"test"};
  assert(j.type() == json::Type::string);
}

void testJsonStringIterator() {
  const json::Json j1{"test"};
  for (const auto ch : j1.chars()) {
    json::log << ch << ' ';
  }
  json::log << std::endl;
  assert(j1.copyString() == "test");
}

void testJsonNonStringIterator() {
  json::Json j;
  try {
    j.chars();
  } catch (std::runtime_error &error) {
    json::log << error.what() << std::endl;
    assert(std::string{error.what()} == "TypeError: not a json string");
  }
}

void testJsonStringOstreamInsertion() {
  json::Json j{"test"};
  std::stringstream ss;
  ss << j;
  json::log << ss.str() << std::endl;
  assert(ss.str() == "test");
}

int main() {
  testDefaultCstr();
  testJsonStrCopyCstr();
  testJsonStrMoveCstr();
  testJsonStringIterator();
  testJsonNonStringIterator();
  testJsonStringOstreamInsertion();
}
