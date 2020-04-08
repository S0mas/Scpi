#include "scpiParser.h"
#include "utils.h"

#include <algorithm>
#include <bitset>
#include <cctype>
#include <sstream>

bool ScpiArgsParser::ScpiArgsRecognizer::isHexFormat(const std::string& str) noexcept {
	return static_cast<char>(std::tolower(static_cast<unsigned char>(str[1]))) == 'h' && isStrUnsignedIntHex(str.substr(2));
}

bool ScpiArgsParser::ScpiArgsRecognizer::isOctFormat(const std::string& str) noexcept {
	return static_cast<char>(std::tolower(static_cast<unsigned char>(str[1]))) == 'o' && isStrUnsignedIntOct(str.substr(2));
}

bool ScpiArgsParser::ScpiArgsRecognizer::isBinFormat(const std::string& str) noexcept {
	return static_cast<char>(std::tolower(static_cast<unsigned char>(str[1]))) == 'b' && isStrUnsignedIntBin(str.substr(2));
}

bool ScpiArgsParser::ScpiArgsRecognizer::isUnsignedLL(const std::string& str) noexcept {
	return str.front() == '#' && str.size() > 2;
}

bool ScpiArgsParser::ScpiArgsRecognizer::isSignedLL(const std::string& str) noexcept {
	return  str.front() == '-' && str.find('.') == std::string::npos;
}

bool ScpiArgsParser::ScpiArgsRecognizer::isString(const std::string& str) noexcept {
	return &str.front() != &str.back() && str.front() == str.back() && (str.front() == '\'' || str.front() == '"');
}

bool ScpiArgsParser::ScpiArgsRecognizer::isList(const std::string& str) noexcept {
	auto strWithoutSpaces = removeSpecifiedChars(str, ' ');
	bool doesInputHaveListStructure = strWithoutSpaces.size() > 3 && strWithoutSpaces[0] == '(' && strWithoutSpaces[1] == '@' && str.back() == ')';
	if (!doesInputHaveListStructure) 
		return false;
	auto listTokens = split(strWithoutSpaces.substr(2, strWithoutSpaces.size() - 3), ',');
	for (const auto& token : listTokens) {
		bool doesListTokenContainInvalidChars = std::find_if(token.begin(), token.end(), [](char c) {return !std::isdigit(static_cast<unsigned char>(c)) && c != ':'; }) != token.end();
		if (doesListTokenContainInvalidChars || token == "" || token[0] == ':' || token.back() == ':' || std::count(token.begin(), token.end(), ':') > 1)
			return false;
	}

	return true;
}

bool ScpiArgsParser::ScpiArgsRecognizer::isMnemonic(const std::string& str) noexcept {
	for (auto const& mnemonic : mnemonics)
		if (mnemonic.first == str)
			return true;
	return false;
}

void ScpiArgsParser::addMnemonic(const std::string& str, ScpiArg const& mappedArg) noexcept {
	ScpiArgsRecognizer::mnemonics[str] = mappedArg;
}

ScpiArg ScpiArgsParser::parseHex(const std::string& str) {
	std::stringstream ss;
	ss << std::hex << str;
	unsigned long long value;
	ss >> value;
	ScpiArg arg;
	arg.set<unsigned long long>(value);
	return arg;
}

ScpiArg ScpiArgsParser::parseOct(const std::string& str) {
	std::stringstream ss;
	ss << std::oct << str;
	unsigned long long value;
	ss >> value;
	ScpiArg arg;
	arg.set<unsigned long long>(value);
	return arg;
}

ScpiArg ScpiArgsParser::parseBin(const std::string& str) {
	std::bitset<64> set(str);
	ScpiArg arg;
	arg.set<unsigned long long>(set.to_ullong());
	return arg;
}

ScpiArg ScpiArgsParser::parseSignedLL(const std::string& str) {
	std::stringstream ss;
	ss << str;
	signed long long value;
	ss >> value;
	ScpiArg arg;
	arg.set<signed long long>(value);
	return arg;
}

ScpiArg ScpiArgsParser::parseDouble(const std::string& str) {
	std::istringstream ss(str);
	double value;
	ss >> value;
	ScpiArg arg;
	arg.set<double>(value);
	return arg;
}

ScpiArg ScpiArgsParser::parseString(const std::string& str) {
	ScpiArg arg;
	arg.set<std::string>(str);
	return arg;
}

ScpiArg ScpiArgsParser::parseList(const std::string& str) {
	auto listWithoutSpaces = removeSpecifiedChars(str, ' ');
	auto listTokens = split(listWithoutSpaces.substr(2, listWithoutSpaces.size() - 3), ',');
	std::vector<unsigned long long> values;
	for (const auto& token : listTokens) {
		if (token.find(':') != std::string::npos) {
			auto numbers = split(token, ':');
			auto range = getRange(std::stoi(numbers.front()), std::stoi(numbers.back()));
			values.reserve(values.capacity() + range.capacity());
			values.insert(values.end(), range.begin(), range.end());
		}
		else
			values.push_back(std::stoi(token));
	}
	ScpiArg arg;
	arg.set<std::vector<unsigned long long>>(values);
	return arg;
}

ScpiArg ScpiArgsParser::parseMnemonic(const std::string& str) {
	return ScpiArgsRecognizer::mnemonics[str];
}

ScpiArg ScpiArgsParser::parseArg(const std::string& toParse) {
	if (toParse.empty())
		throw EmptyArgDataException();
	if (ScpiArgsRecognizer::isUnsignedLL(toParse)) {
		if (ScpiArgsRecognizer::isHexFormat(toParse))
			return parseHex(toParse.substr(2));
		else if (ScpiArgsRecognizer::isOctFormat(toParse))
			return parseOct(toParse.substr(2));
		else if (ScpiArgsRecognizer::isBinFormat(toParse))
			return parseBin(toParse.substr(2));
	}
	else if (isStrValidDouble(toParse))
		return parseDouble(toParse);
	else if (ScpiArgsRecognizer::isSignedLL(toParse))
		return parseSignedLL(toParse);
	else if (ScpiArgsRecognizer::isString(toParse))
		return parseString(toParse.substr(1, toParse.size() - 2));
	else if (ScpiArgsRecognizer::isList(toParse))
		return parseList(toParse);
	else if (ScpiArgsRecognizer::isMnemonic(toParse))
		return parseMnemonic(toParse);

	throw InvalidArgDataException();
}


std::vector<ScpiArg> ScpiParser::parseArgs(const std::vector<std::string>& args) {
	std::vector<ScpiArg> result;
	for (auto const& arg : args)
		result.push_back(ScpiArgsParser::parseArg(arg));

	return result;
}

std::string ScpiParser::getCmdPart(const std::string& data) {
	auto argsStartingPosition = data.find(' ');
	return (argsStartingPosition != std::string::npos) ? data.substr(0, argsStartingPosition) : data;
}

std::string ScpiParser::getArgsPart(const std::string& data) {
	auto argsStartingPosition = data.find(' ');
	return (argsStartingPosition != std::string::npos) ? data.substr(argsStartingPosition) : "";
}

std::string ScpiParser::removeLeadingColon(const std::string& data) {
	return !data.empty() && data[0] == ':' ? data.substr(1) : data;
}

std::vector<unsigned long long> ScpiParser::getInCmdNameNumberParams(const std::vector<std::string>& cmds) {
	std::vector<unsigned long long> inCmdNameNubmberParams;
	for(auto const& cmd : cmds)
		if (!cmd.empty() && isdigit(cmd.back()))
			inCmdNameNubmberParams.push_back(getUnsignedIntegerFromStringEnd(cmd));
	return inCmdNameNubmberParams;
}

ScpiArg ScpiParser::parseArgsFromCmdNames(const std::vector<unsigned long long>& inNameParams) {
	ScpiArg arg;
	arg.set<std::vector<unsigned long long>>(inNameParams);
	return arg;
}

ParsedScpiData ScpiParser::parse(const std::string& data) {
	auto scpiCmdNames = split(removeLeadingColon(getCmdPart(data)), ':');

	auto splitedArgsTokens = split(getArgsPart(data), ',');	
	auto scpiArgs = concatenateSplitedTokenParts(splitedArgsTokens, ',', '\'', '\'');//strings parts
	scpiArgs = concatenateSplitedTokenParts(scpiArgs, ',', '(', ')');//lists parts
	for (auto& token : scpiArgs)
		token = removeLeadingSpecifiedChars(removeEndingSpecifiedChars(token, ' '), ' ');
	auto parsedArgs = parseArgs(scpiArgs);

	auto inNameParams = getInCmdNameNumberParams(scpiCmdNames);
	if(!inNameParams.empty())
		parsedArgs.push_back(parseArgsFromCmdNames(inNameParams));
	
	return { scpiCmdNames, parsedArgs };
}
