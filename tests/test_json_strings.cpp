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
  auto chars = j1.chars();
  assert(std::string(chars.cbegin(), chars.cend()) == "test");
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

void testHeldTypeMutability() {
  json::Json j;
  json::log << j << std::endl;
  json::Json j1{"test"};
  j = j1;
  json::log << j << std::endl;
}

void testDumps() {
  json::Json j = "test str: こんにちは\n";
  auto str = j.dumps();
  json::log << "dumped string is: " << str << std::endl;
  assert(str == "\"test str: \\u3053\\u3093\\u306b\\u3061\\u306f\\n\"");
}

int main() {
  testDefaultCstr();
  testJsonStrCopyCstr();
  testJsonStrMoveCstr();
  testJsonStringIterator();
  testJsonNonStringIterator();
  testJsonStringOstreamInsertion();
  testHeldTypeMutability();
  testDumps();
}
