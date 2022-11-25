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
  for (auto ch{j.s_cbegin()}; ch != j.s_cend(); ++ch) {
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
