
#include "json.h"
#include "test.h"
#include <cassert>

void testFloat() {
  using json::Json;
  Json num = 1234.5678;
  json::log << num << std::endl;
  assert(num.toFloatingPoint() == 1234.5678);
}

void testLoads() {
  using json::Json;
  std::string str = "1234.123";
  try {
    Json js = Json::loads(str);
    assert(js.toFloatingPoint() == 1234.123);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

void testLoadsNegativeNumber() {
  using json::Json;
  std::string str = "-1234.1234";
  try {
    Json js = Json::loads(str);
    assert(js.toFloatingPoint() == -1234.1234);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

int main() {
  testFloat();
  testLoads();
  testLoadsNegativeNumber();
}
