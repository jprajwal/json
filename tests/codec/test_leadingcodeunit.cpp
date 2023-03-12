#include "json/codec/utf8leadingcodeunit.h"
#include "json/test.h"
#include <algorithm>
#include <assert.h>
#include <ios>
#include <utility>
#include <vector>

template <typename T>
void assertContainersAreEqual(const std::vector<T> &lhs,
                              const std::vector<T> &rhs) {
  assert(std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()));
}

void test1() {
  std::vector<int> arr{0x61, 0x00, 0x7F, 0x61, 0x80};
  std::vector<bool> actual{};

  std::vector<bool> expected{true, true, true, true, false};

  std::for_each(std::begin(arr), std::end(arr), [=, &actual](const auto item) {
    json::codec::LeadingCodeUnit<char> lcu{item};
    json::log << std::boolalpha << lcu.isValid() << std::endl;
    actual.push_back(lcu.isValid());
  });
  assertContainersAreEqual(expected, actual);
}

void test2() {}

int main() {
  test1();
  test2();
}