#pragma once

#include <vector>
#include <string>

bool isVowel(const char& letter) noexcept;
std::vector<std::string> split(const std::string& str, char delimiter) noexcept;
std::string removeSpecifiedChars(const std::string& data, const char& toBeRemoved);
std::string removeLeadingSpecifiedChars(const std::string& data, const char& toBeRemoved);
std::string removeEndingSpecifiedChars(const std::string& data, const char& toBeRemoved);
bool isStrValidDouble(const std::string& s) noexcept;
unsigned long long getUnsignedIntegerFromStringEnd(const std::string& str);
unsigned long long getNumberOfDigits(unsigned long long number) noexcept;
std::vector<int> getRange(const int lhs, const int rhs);
bool isStrUnsignedIntBin(const std::string& str) noexcept;
bool isStrUnsignedIntOct(const std::string& str) noexcept;
bool isStrUnsignedIntDec(const std::string& str) noexcept;
bool isStrUnsignedIntHex(const std::string& str) noexcept;
std::vector<std::string> concatenateSplitedTokenParts(const std::vector<std::string>& tokens, const char symbolUsedToSplitTokens, const char tokenBeginSymbol, const char tokenEndSymbol);