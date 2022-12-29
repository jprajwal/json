
#include "json.h"
#include "test.h"
#include <cassert>

void testFloat() {
  using json::Json;
  Json num = 1234.5678;
  json::log << num << std::endl;
  assert(num.toFloatingPoint() == 1234.5678);
}

int main() { testFloat(); }
