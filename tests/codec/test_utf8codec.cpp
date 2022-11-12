
#include "codec/utf8codec.h"
#include "json.h"

#include <bitset>
#include <iostream>
#include <string>
#include <vector>

template <typename T> void printVector(std::vector<T> &vec) {
  std::cout << '[';
  for (const auto &item : vec) {
    std::cout << item << ',';
  }
  std::cout << ']';
  std::cout << std::endl;
}

std::vector<std::string>
convertRuneToString(std::vector<json::codec::Rune> &vec) {
  std::vector<std::string> strArray{};
  for (const auto &rune : vec) {
    strArray.push_back(std::to_string(rune));
  }
  return strArray;
}

std::vector<char> convertStringToVector(std::string &str) {
  std::vector<char> byteArray{};
  for (const auto &ch : str) {
    byteArray.push_back(ch);
  }
  return byteArray;
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

void testDecoding(std::string &str) {
  auto byteArray{convertStringToVector(str)};
  printVector<char>(byteArray);
  auto binaryArray{convertByteArrayToBitSetArray(byteArray)};
  printVector<std::bitset<8>>(binaryArray);
  json::codec::UTF8Codec codec{};
  auto runeArray{codec.decode(byteArray)};
  printVector<json::codec::Rune>(runeArray);
}

void testUTF8CodecSimpleASCIIDecoding() {
  std::string str{"hello"};
  testDecoding(str);
}

void testUTF8CodecJapaneseDecoding() {
  std::string str{"こんにちは"};
  testDecoding(str);
}

int main() {
  testUTF8CodecSimpleASCIIDecoding();
  testUTF8CodecJapaneseDecoding();
}