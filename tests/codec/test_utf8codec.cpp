
#include "codec/utf8codec.h"
#include "json.h"
#include "test.h"

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec) {
  out << '[';
  for (const auto &item : vec) {
    out << item << ',';
  }
  out << ']';
  return out;
}

template <>
std::ostream &operator<< <char>(std::ostream &out,
                                const std::vector<char> &vec) {
  out << '[';
  for (const auto &item : vec) {
    out << static_cast<std::uint16_t>(item) << ',';
  }
  out << ']';
  return out;
}

std::vector<std::bitset<8>>
convertByteArrayToBitSetArray(std::vector<char> &vec) {
  std::vector<std::bitset<8>> binaryArray{};
  for (const auto &ch : vec) {
    std::bitset<8> binary{static_cast<unsigned long long>(ch)};
    binaryArray.push_back(binary);
  }
  return binaryArray;
}

template <typename T>
void assertContainersAreEqual(const std::vector<T> &lhs,
                              const std::vector<T> &rhs) {
  assert(std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()));
}

template <typename Codec>
void testDecoding(std::string &str,
                  const std::vector<json::codec::Rune> &expectedRuneArray) {
  Codec codec{};
  auto byteArray{std::vector(str.cbegin(), str.cend())};
  auto binaryArray{convertByteArrayToBitSetArray(byteArray)};
  auto runeArray{codec.decode(byteArray)};
  json::log << "For string: " << str << std::endl;
  json::log << "-> byte array: " << std::hex << byteArray << std::endl;
  json::log << "-> bitset array: " << binaryArray << std::endl;
  json::log << "-> rune array: " << std::hex << runeArray << std::endl;
  assertContainersAreEqual<json::codec::Rune>(runeArray, expectedRuneArray);
}

// https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
void testUTF8CodecSimpleASCIIDecoding() {
  std::string str{"hello"};
  std::vector<json::codec::Rune> expected{0x68, 0x65, 0x6c, 0x6c, 0x6f};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

void testUTF8CodecJapaneseDecoding() {
  std::string str{"こんにちは"};
  std::vector<json::codec::Rune> expected{0x3053, 0x3093, 0x306B, 0x3061,
                                          0x306F};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

void testUTF8CodecGreekWordDecoding() {
  std::string str{"κόσμε"};
  std::vector<json::codec::Rune> expected{0x3BA, 0x1F79, 0x3C3, 0x3BC, 0x3B5};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

void testUTF8CodecFirstPossibleSeqOfOneByteLength() {
  std::string str{0x00};
  std::vector<json::codec::Rune> expected{0x00000000};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

void testUTF8CodecFirstPossibleSeqOfTwoByteLength() {
  // std::string str{0b1100'0001, 0b1000'0000};
  std::string str{""};
  std::vector<json::codec::Rune> expected{0x00000080};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

void testUTF8CodecFirstPossibleSeqOfThreeByteLength() {
  // std::string str{0b1100'0001, 0b1000'0000};
  std::string str{"ࠀ"};
  std::vector<json::codec::Rune> expected{0x00000800};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

void testUTF8CodecFirstPossibleSeqOfFourByteLength() {
  // std::string str{0b1100'0001, 0b1000'0000};
  std::string str{"𐀀"};
  std::vector<json::codec::Rune> expected{0x00010000};
  testDecoding<json::codec::UTF8Codec>(str, expected);
}

int main() {
  testUTF8CodecSimpleASCIIDecoding();
  testUTF8CodecJapaneseDecoding();
  testUTF8CodecGreekWordDecoding();
  testUTF8CodecFirstPossibleSeqOfOneByteLength();
  testUTF8CodecFirstPossibleSeqOfTwoByteLength();
  testUTF8CodecFirstPossibleSeqOfThreeByteLength();
  testUTF8CodecFirstPossibleSeqOfFourByteLength();
}