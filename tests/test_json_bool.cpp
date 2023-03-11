

#include "json.h"
#include "test.h"
#include <cassert>

void testBool() {
  using json::Json;
  Json num = true;
  json::log << num << std::endl;
  assert(num.toBoolean() == true);
}

void testLoadsTrue() {
  using json::Json;
  std::string test = R"(true)";
  try {
    Json result = Json::loads(test);
    json::log << "loaded boolean: " << result << std::endl;
    assert(result.toBoolean() == true);
  } catch (std::exception &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

void testLoadsFalse() {
  using json::Json;
  std::string test = R"(false)";
  try {
    Json result = Json::loads(test);
    json::log << "loaded boolean: " << result << std::endl;
    assert(result.toBoolean() == false);
  } catch (std::exception &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

int main() {
  testBool();
  testLoadsTrue();
  testLoadsFalse();
}
