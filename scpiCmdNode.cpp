#include "scpiCmdNode.h"
#include "utils.h"

#include <algorithm>
#include <cctype>

ScpiCmdNode::ScpiCmdNode(const std::string& full, const ScpiFunction& scpiFun, const bool canBeUsedWithInNameNumberParam, const unsigned int maxInNameNumberParam)
	: canBeUsedWithInNameNumberParam(canBeUsedWithInNameNumberParam),
	maxInNameNumberParam(maxInNameNumberParam),
	scpiFun(scpiFun) {
	fullName = full;
	createShortName();
}

void ScpiCmdNode::createShortName() noexcept {
	std::transform(fullName.begin(), fullName.end(), fullName.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	if (fullName.size() <= 4 || (fullName.size() == 5 && fullName.back() == '?'))
		shortName = fullName;
	else {
		shortName = fullName.substr(0, 4);
		if (isVowel(shortName.back()))
			shortName.pop_back();
	}
	if (!shortName.empty() && shortName.back() != '?' && fullName.back() == '?')
		shortName.push_back('?');
}

ScpiCmdNode* ScpiCmdNode::findCmd(const std::string& name) noexcept {
	for (auto& node : children)
		if(*node.get() == name)
			return node.get();
	return nullptr;
}

ScpiCmdNode* ScpiCmdNode::findCmd(const std::vector<std::string>& names) noexcept {
	ScpiCmdNode* navigator = this;
	for (auto const& name : names) {
		navigator = navigator->findCmd(name);
		if (!navigator)
			return nullptr;
	}
	return navigator;
}

int ScpiCmdNode::run(const std::vector<ScpiArg>& args) {
	return scpiFun.run(args);
}

std::string ScpiCmdNode::getShortName() const {
	return shortName;
}

std::string ScpiCmdNode::getFullName() const {
	return fullName;
}

static bool validateInNameNumberParam(const unsigned int maxInNameNumberParam, const std::string& str) noexcept {
	try {
		unsigned int number = std::stoi(str);
		return getNumberOfDigits(maxInNameNumberParam) >= str.size() && maxInNameNumberParam >= number && getNumberOfDigits(number) == str.size() && number != 0;
	}
	catch (...) {
		return false;
	}
}

static bool validateCmdNameThatCanBeUsedWithInNameParam(const std::string& name, const std::string& str, const unsigned int maxInNameNumberParam) noexcept {
	if (str.size() > name.size() && str.substr(0, name.size()) == name)
		return validateInNameNumberParam(maxInNameNumberParam, str.substr(name.size()));
	return false;
}

static void toStringH(const ScpiCmdNode* node, int& deep, std::string& result) {
	result += node->getFullName();
	++deep;
	for (auto const& node : node->children) {
		result += '\n';
		for (auto i = 0; i < deep; ++i)
			result += '\t';
		result += ':';
		toStringH(node.get(), deep, result);
	}
	--deep;
}

std::string ScpiCmdNode::toString() const noexcept {
	int deep = 0;
	std::string result;
	toStringH(this, deep, result);
	return result;
}

bool operator==(const ScpiCmdNode& node, const std::string& str) noexcept {
	auto tmp = str;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

	bool isValidCmdWithInNameNumberParam = false;
	if (node.canBeUsedWithInNameNumberParam)
		isValidCmdWithInNameNumberParam = validateCmdNameThatCanBeUsedWithInNameParam(node.getFullName(), tmp, node.maxInNameNumberParam) || validateCmdNameThatCanBeUsedWithInNameParam(node.getShortName(), tmp, node.maxInNameNumberParam);
	return isValidCmdWithInNameNumberParam || tmp == node.getFullName() || tmp == node.getShortName();
}

bool operator==(const std::string& str, const ScpiCmdNode& node) noexcept {
	return node == str;
}

bool operator!=(const ScpiCmdNode& node, const std::string& str) noexcept {
	return !(node == str);
}

bool operator!=(const std::string& str, const ScpiCmdNode& node) noexcept {
	return node != str;
}
