#include "json/json.h"
#include "json/json_decode_error.h"
#include "json/test.h"
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

void testLoads() {
  using json::Json;
  std::string str = "1234";
  try {
    Json js = Json::loads(str);
    assert(js.toInteger() == 1234l);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

void testLoadsNegativeNumber() {
  using json::Json;
  std::string str = "-1234";
  try {
    Json js = Json::loads(str);
    assert(js.toInteger() == -1234l);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

int main() {
  testInit();
  testStaticCasting();
  testDumps();
  testLoads();
  testLoadsNegativeNumber();
}
