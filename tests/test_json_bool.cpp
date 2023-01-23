

#include "json.h"
#include "test.h"
#include <cassert>

void testBool() {
  using json::Json;
  Json num = true;
  json::log << num << std::endl;
  assert(num.toBoolean() == true);
}

int main() { testBool(); }
