#include "json.h"
#include "test.h"
#include <cassert>

void testInit() {
  using json::Json;
  Json num = 1234l;
  json::log << num << std::endl;
  assert(num.toInteger() == 1234);
}

void testStaticCasting() {
  using json::Json;
  Json num = 1234l;
  Json::int_t n = static_cast<Json::int_t>(num);
  json::log << n << std::endl;
  assert(n == 1234);
}

void testDumps() {
  using json::Json;
  Json n = 1234l;
  auto str = n.dumps();
  json::log << "dumped integer: " << str << std::endl;
  assert(str == "1234");
}

int main() {
  testInit();
  testStaticCasting();
  testDumps();
}
