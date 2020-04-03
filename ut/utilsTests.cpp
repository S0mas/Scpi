#include "gtest/gtest.h"
#include "../utils.h"

TEST(utils, isVowel) {
	EXPECT_TRUE(isVowel('a'));
	EXPECT_TRUE(isVowel('e'));
	EXPECT_TRUE(isVowel('i'));
	EXPECT_TRUE(isVowel('o'));
	EXPECT_TRUE(isVowel('u'));
	EXPECT_TRUE(isVowel('y'));
	EXPECT_TRUE(isVowel('A'));
	EXPECT_TRUE(isVowel('E'));
	EXPECT_TRUE(isVowel('I'));
	EXPECT_TRUE(isVowel('O'));
	EXPECT_TRUE(isVowel('U'));
	EXPECT_TRUE(isVowel('Y'));
	EXPECT_FALSE(isVowel('R'));
	EXPECT_FALSE(isVowel('t'));
	EXPECT_FALSE(isVowel('Z'));
	EXPECT_FALSE(isVowel('x'));
	EXPECT_FALSE(isVowel('C'));
	EXPECT_FALSE(isVowel('v'));
}

TEST(utils, split) {
	std::string str("first,second,next.next,4th");

	auto tokens = split(str, ',');
	EXPECT_EQ(tokens.size(), 4);
	EXPECT_EQ(tokens[0], "first");
	EXPECT_EQ(tokens[1], "second");
	EXPECT_EQ(tokens[2], "next.next");
	EXPECT_EQ(tokens.back(), "4th");
}

TEST(utils, split2) {
	std::string str("first,,thrid");

	auto tokens = split(str, ',');
	EXPECT_EQ(tokens.size(), 3);
	EXPECT_EQ(tokens[0], "first");
	EXPECT_EQ(tokens[1], "");
	EXPECT_EQ(tokens.back(), "thrid");
}

TEST(utils, removeSpecifiedChars) {
	std::string str("first,second,next.next,4th");
	EXPECT_EQ(removeSpecifiedChars(str, ','), "firstsecondnext.next4th");
	EXPECT_EQ(removeSpecifiedChars(str, '.'), "first,second,nextnext,4th");
	EXPECT_EQ(removeSpecifiedChars(str, 'D'), str);
}

TEST(utils, isStrValidDouble) {
	EXPECT_TRUE(isStrValidDouble("+41.25"));
	EXPECT_TRUE(isStrValidDouble("-0.25"));
	EXPECT_TRUE(isStrValidDouble("1111.25"));
	EXPECT_TRUE(isStrValidDouble("+41"));
	EXPECT_TRUE(isStrValidDouble("-3312"));
	EXPECT_TRUE(isStrValidDouble("100"));

	EXPECT_FALSE(isStrValidDouble("--3312"));
	EXPECT_FALSE(isStrValidDouble("++100"));
	EXPECT_FALSE(isStrValidDouble("15.1."));
	EXPECT_FALSE(isStrValidDouble("15..1.31"));
}

TEST(utils, getUnsignedIntegerFromStringEnd) {
	EXPECT_EQ(getUnsignedIntegerFromStringEnd("adammmm12"), 12);
	EXPECT_EQ(getUnsignedIntegerFromStringEnd("ada mmmm-44"), 44);
	EXPECT_EQ(getUnsignedIntegerFromStringEnd("adammmm31"), 31);
	EXPECT_EQ(getUnsignedIntegerFromStringEnd("ada244mmmm44-2"), 2);
	EXPECT_EQ(getUnsignedIntegerFromStringEnd("ada-13mmmm177"), 177);
	EXPECT_EQ(getUnsignedIntegerFromStringEnd("a223dammmm000054"), 54);
}

TEST(utils, getNumberOfDigits) {
	EXPECT_EQ(getNumberOfDigits(10), 2);
	EXPECT_EQ(getNumberOfDigits(0), 1);
	EXPECT_EQ(getNumberOfDigits(1), 1);
	EXPECT_EQ(getNumberOfDigits(1151), 4);
}

TEST(utils, getRange) {
	auto range = getRange(1, 1);
	EXPECT_EQ(range.size(), 1);
	EXPECT_EQ(range.front(), 1);

	range = getRange(-1, 0);
	EXPECT_EQ(range.size(), 2);
	EXPECT_EQ(range.front(), -1);
	EXPECT_EQ(range.back(), 0);

	range = getRange(0, -1);
	EXPECT_EQ(range.size(), 2);
	EXPECT_EQ(range.front(), 0);
	EXPECT_EQ(range.back(), -1);
}

TEST(utils, isStrUnsignedIntBin) {
	EXPECT_TRUE(isStrUnsignedIntBin("10"));
	EXPECT_TRUE(isStrUnsignedIntBin("1010"));
	EXPECT_TRUE(isStrUnsignedIntBin("1111"));
	EXPECT_TRUE(isStrUnsignedIntBin("1000"));

	EXPECT_FALSE(isStrUnsignedIntBin("211"));
	EXPECT_FALSE(isStrUnsignedIntBin("101b"));
	EXPECT_FALSE(isStrUnsignedIntBin("b1011"));
}

TEST(utils, isStrUnsignedIntOct) {
	EXPECT_TRUE(isStrUnsignedIntOct("777"));
	EXPECT_TRUE(isStrUnsignedIntOct("1073"));
	EXPECT_TRUE(isStrUnsignedIntOct("1"));
	EXPECT_TRUE(isStrUnsignedIntOct("774"));

	EXPECT_FALSE(isStrUnsignedIntOct("o765"));
	EXPECT_FALSE(isStrUnsignedIntOct("765o"));
	EXPECT_FALSE(isStrUnsignedIntOct("86245"));
}

TEST(utils, isStrUnsignedIntDec) {
	EXPECT_TRUE(isStrUnsignedIntDec("19"));
	EXPECT_TRUE(isStrUnsignedIntDec("1910"));
	EXPECT_TRUE(isStrUnsignedIntDec("9999"));
	EXPECT_TRUE(isStrUnsignedIntDec("89557"));

	EXPECT_FALSE(isStrUnsignedIntDec("A11"));
	EXPECT_FALSE(isStrUnsignedIntDec("101d"));
	EXPECT_FALSE(isStrUnsignedIntDec("d1011"));
}

TEST(utils, isStrUnsignedIntHex) {
	EXPECT_TRUE(isStrUnsignedIntHex("Ac"));
	EXPECT_TRUE(isStrUnsignedIntHex("bDBd"));
	EXPECT_TRUE(isStrUnsignedIntHex("0XFbfa"));
	EXPECT_TRUE(isStrUnsignedIntHex("0xCe0E"));

	EXPECT_FALSE(isStrUnsignedIntHex("0x"));
	EXPECT_FALSE(isStrUnsignedIntHex("101x"));
	EXPECT_FALSE(isStrUnsignedIntHex("x1011"));
}

TEST(utils, removeLeadingSpecifiedChars) {
	EXPECT_EQ("d  upa    ", removeLeadingSpecifiedChars("    d  upa    ", ' '));
	EXPECT_EQ("", removeLeadingSpecifiedChars("", ' '));
	EXPECT_EQ("", removeLeadingSpecifiedChars("", ' '));
}

TEST(utils, removeEndingSpecifiedChars) {
	EXPECT_EQ("    d  upa", removeEndingSpecifiedChars("    d  upa    ", ' '));
	EXPECT_EQ("", removeEndingSpecifiedChars("", ' '));
	EXPECT_EQ("", removeEndingSpecifiedChars("aaa", 'a'));
}

TEST(utils, concatenateOneTokenParts) {
	std::string fullToken = "'  abc,  ,  dfr , ,'";
	auto symbolUsedToSplitTokens = ',';
	auto tokenParts = split(fullToken, symbolUsedToSplitTokens);
	auto concatenateTokens = concatenateSplitedTokenParts(tokenParts, symbolUsedToSplitTokens, '\'', '\'');
	EXPECT_EQ(concatenateTokens.size(), 1);
	EXPECT_EQ(concatenateTokens[0], fullToken);
}

TEST(utils, concatenateTwoTokenParts) {
	std::string tokens = "'  abc,  ,  dfr , ,', ' ds  ,ab,c d ,'";
	auto symbolUsedToSplitTokens = ',';
	auto tokenParts = split(tokens, symbolUsedToSplitTokens);
	auto concatenateTokens = concatenateSplitedTokenParts(tokenParts, symbolUsedToSplitTokens, '\'', '\'');
	EXPECT_EQ(concatenateTokens.size(), 2);
	EXPECT_EQ(concatenateTokens[0], "'  abc,  ,  dfr , ,'");
	EXPECT_EQ(concatenateTokens[1], " ' ds  ,ab,c d ,'");
}


TEST(utils, concatenateEmpty) {
	std::string data = "";
	auto symbolUsedToSplitTokens = ',';
	auto tokenParts = split(data, symbolUsedToSplitTokens);
	EXPECT_TRUE(tokenParts.empty());
	auto concatenateTokens = concatenateSplitedTokenParts(tokenParts, symbolUsedToSplitTokens, '(', ')');
	EXPECT_TRUE(concatenateTokens.empty());
}

TEST(utils, concatenateWithoutTokensSymbols) {
	std::string data = "FIRST_CMD_NODE #h0x11, @2:3, 6, 1, -444";
	auto symbolUsedToSplitTokens = ',';
	auto tokenParts = split(data, symbolUsedToSplitTokens);
	EXPECT_EQ(tokenParts.size(), 5);
	auto concatenateTokens = concatenateSplitedTokenParts(tokenParts, symbolUsedToSplitTokens, '(', ')');
	EXPECT_EQ(concatenateTokens, tokenParts);
}

TEST(utils, concatenateOneTokenParts1) {
	std::string fullToken = " (@2:3, 6, 1)";
	auto symbolUsedToSplitTokens = ',';
	std::vector<std::string> tokens = {" (@2:3"," 6"," 1)" };
	auto concatenateTokens = concatenateSplitedTokenParts(tokens, symbolUsedToSplitTokens, '(', ')');
	EXPECT_EQ(concatenateTokens.size(), 1);
	EXPECT_EQ(concatenateTokens[0], fullToken);
}

TEST(utils, concatenateOneTokenParts2) {
	std::string fullToken = " (@2:3, 6, 1)";
	auto symbolUsedToSplitTokens = ',';
	std::vector<std::string> tokens = { "d", " (@2:3"," 6"," 1)" };
	auto concatenateTokens = concatenateSplitedTokenParts(tokens, symbolUsedToSplitTokens, '(', ')');
	EXPECT_EQ(concatenateTokens.size(), 2);
	EXPECT_EQ(concatenateTokens[1], fullToken);
}

TEST(utils, concatenateOneTokenParts3) {
	std::string fullToken = " (@2:3, 6, 1)";
	auto symbolUsedToSplitTokens = ',';
	std::vector<std::string> tokens = { "d", " (@2:3"," 6"," 1)", "abs" };
	auto concatenateTokens = concatenateSplitedTokenParts(tokens, symbolUsedToSplitTokens, '(', ')');
	EXPECT_EQ(concatenateTokens.size(), 3);
	EXPECT_EQ(concatenateTokens[1], fullToken);
}
