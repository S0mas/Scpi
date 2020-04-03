#include "utils.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iterator>
#include <numeric>
#include <sstream>

bool isVowel(const char& letter) noexcept {
	auto lowerLetter = static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));
	return lowerLetter == 'a' || lowerLetter == 'e' || lowerLetter == 'i' || lowerLetter == 'o' || lowerLetter == 'u' || lowerLetter == 'y';
}

std::vector<std::string> split(const std::string& str, char delimiter) noexcept {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);
	return tokens;
}

std::string removeSpecifiedChars(const std::string& data, const char& toBeRemoved) {
	auto result = data;
	result.erase(std::remove(result.begin(), result.end(), toBeRemoved),
		result.end());
	return result;
}

std::string removeLeadingSpecifiedChars(const std::string& data, const char& toBeRemoved) {
	auto result = data;
	auto it = std::find_if(result.begin(), result.end(), [&toBeRemoved](const auto& c) { return c != toBeRemoved; });
	result.erase(result.begin(), it);
	return result;
}
std::string removeEndingSpecifiedChars(const std::string& data, const char& toBeRemoved) {
	auto result = data;
	auto it = std::find_if(result.rbegin(), result.rend(), [&toBeRemoved](const auto& c) { return c != toBeRemoved; });
	result.erase(it.base(), result.end());
	return result;
}

bool isStrValidDouble(const std::string& s) noexcept {
	char* end = 0;
	double val = strtod(s.c_str(), &end);

	if (end != s.c_str()+s.size())
		return false;
	return end != s.c_str() && val != HUGE_VAL ;
}

unsigned long long getUnsignedIntegerFromStringEnd(const std::string& str) {
	unsigned long long n = 1;
	std::string result;
	auto backIt = str.crbegin();
	while (backIt != str.crend() && isdigit(*backIt)) {
		result += *backIt;
		++backIt;
	}
	std::reverse(result.begin(), result.end());
	return std::stoi(result);
}

unsigned long long getNumberOfDigits(unsigned long long number) noexcept {
	unsigned long long digits = 0;
	do  {
		number /= 10;
		++digits;
	} while (number);
	return digits;
}

std::vector<int> getRange(const int lhs, const int rhs) {
	std::vector<int> values;
	values.reserve(std::abs(lhs - rhs) + 1);
	int step = lhs <= rhs ? 1 : -1;
	for (auto i = lhs; i != rhs+step; i += step)
		values.push_back(i);
	return values;
}
static bool isStrUnsignedInt(const std::string& str, const std::string& validSymbols) noexcept {
	return !str.empty() && str.find_first_not_of(validSymbols) == std::string::npos;
}

bool isStrUnsignedIntBin(const std::string& str) noexcept {
	return isStrUnsignedInt(str, "01");
}

bool isStrUnsignedIntOct(const std::string& str) noexcept {
	return isStrUnsignedInt(str, "01234567");
}

bool isStrUnsignedIntDec(const std::string& str) noexcept {
	return isStrUnsignedInt(str, "0123456789");
}
bool isStrUnsignedIntHex(const std::string& str) noexcept {
	return isStrUnsignedInt(str, "0123456789ABCDEFabcdef") || str.size()>2 && str.front() == '0' && static_cast<char>(std::tolower(static_cast<unsigned char>(str[1]))) == 'x' && isStrUnsignedInt(str.substr(2), "0123456789ABCDEFabcdef");
}

std::vector<std::string> concatenateSplitedTokenParts(const std::vector<std::string>& tokens, const char charUsedToSplitTokens,const char tokenBeginSymbol, const char tokenEndSymbol) {
	std::vector<std::string> result;
	result.reserve(tokens.size());
	auto it = tokens.begin();

	while (it != tokens.end()) {
		auto beginSymbolPosition = std::string::npos;
		auto startIt = std::find_if(it, tokens.end(), [&tokenBeginSymbol, &beginSymbolPosition](auto const& token) 
			{  
				beginSymbolPosition = token.find(tokenBeginSymbol);
				return beginSymbolPosition != std::string::npos; 
			});
		//add all tokens we passed
		result.insert(result.end(), std::make_move_iterator(it), std::make_move_iterator(startIt));
		if (startIt == tokens.end())
			return result;

		//check if its already fullToken
		if (startIt->size() > beginSymbolPosition && startIt->find(tokenEndSymbol, beginSymbolPosition + 1) != std::string::npos) {
			result.push_back(*startIt);
			it = std::next(startIt);
			continue;
		}

		auto endIt = std::find_if(std::next(startIt), tokens.end(), [&tokenEndSymbol](auto const& token) { return token.find(tokenEndSymbol) != std::string::npos; });

		//if endIt is not pass end iterator we need to advance it (to be included in fulltoken)
		if (endIt != tokens.end())
			std::advance(endIt, 1);

		//create fulltToken
		std::string fullToken = "";
		for (auto it = startIt; it != endIt; ++it)
			fullToken +=  *it + charUsedToSplitTokens;
		fullToken.pop_back();

		result.push_back(fullToken);
		it = endIt;
	}
	return result;
}
