#include "json/json.h"
#include "json/test.h"
#include <cassert>
#include <iterator>

void testInit() {
  using json::Json;
  Json js = Json::list_t{{"hello", 1l}};
  json::log << "json list: " << js << std::endl;
  assert(js.isList());
  assert(js[0] == Json{"hello"});
  assert(js[1] == Json{1l});
}

void testIterator() {
  using json::Json;
  Json js = Json::list_t{{"hello", 1l}};
  auto iter = js.list_iter();
  auto &first_element = *iter;
  auto &second_element = *std::next(iter);
  json::log << "first_element: " << first_element << std::endl;
  json::log << "second_element: " << second_element << std::endl;
  assert(first_element == Json{"hello"});
  assert(second_element == Json{1l});
}

void testDumps() {
  using json::Json;
  Json js = Json::list_t{{"hello", {{"hello", "hello", {{"k1", "v1"}}}}}};
  auto result = js.dumps();
  json::log << "dumped list: " << result << std::endl;
  assert(result == R"(["hello", ["hello", "hello", {"k1": "v1"}]])");
}

void testLoads() {
  using json::Json;
  std::string test = R"(["hello", 1234, {"k1": "v1"}])";
  try {
    auto result = Json::loads(test);
    json::log << "loaded list: " << result << std::endl;
    assert(result[0] == "hello");
    assert(result[1] == 1234l);
    auto expectedMap = Json::object_t{{{"k1", "v1"}}};
    assert(result[2] == expectedMap);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

void testLoadsEmptyList() {
  using json::Json;
  std::string test = R"([])";
  try {
    auto result = Json::loads(test);
    json::log << "loaded list: " << result << std::endl;
    assert(result.toList().empty());
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(false);
  }
}

void testLoadsExtraComma() {
  using json::Json;
  std::string test = R"(["hello",])";
  try {
    auto result = Json::loads(test);
    json::log << "loaded list: " << result << std::endl;
    assert(false);
  } catch (json::JsonDecodeError &exc) {
    json::log << exc.what() << std::endl;
    assert(true);
  }
}

int main() {
  testInit();
  testIterator();
  testDumps();
  testLoads();
  testLoadsEmptyList();
  testLoadsExtraComma();
}