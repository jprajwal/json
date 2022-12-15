#include "json.h"
#include "test.h"
#include <cassert>

void testInit() {
  using json::Json;
  Json num{1234};
  json::log << num << std::endl;
  assert(num.toInteger() == 1234);
}

void testStaticCasting() {
  using json::Json;
  Json num{1234};
  Json::int_t n = static_cast<Json::int_t>(num);
  json::log << n << std::endl;
  assert(n == 1234);
}

int main() {
  testInit();
  testStaticCasting();
}
