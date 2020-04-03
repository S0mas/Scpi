#pragma once
#include "scpiFunction.h"

#include <memory>
#include <string>

class ScpiCmdNode {
	std::string fullName;
	std::string shortName;
	ScpiFunction scpiFun;

	ScpiCmdNode* findCmd(const std::string& name) noexcept;
	void createShortName() noexcept;
public:
	const bool canBeUsedWithInNameNumberParam;
	const unsigned int maxInNameNumberParam;
	std::vector<std::unique_ptr<ScpiCmdNode>> children;

	ScpiCmdNode(const std::string& full, const ScpiFunction& scpiFun = ScpiFunction::getScpiFunction<>(), const bool canBeUsedWithInNameNumberParam = false, const unsigned int maxInNameNumberParam = 0);
	ScpiCmdNode* findCmd(const std::vector<std::string>& names) noexcept;
	int run(const std::vector<ScpiArg>& args);
	std::string getShortName() const;
	std::string getFullName() const;
	bool isFunctionDefined() const noexcept {
		return scpiFun.isRunable();
	};
	std::string toString() const noexcept;
};

bool operator==(const ScpiCmdNode& node, const std::string& str) noexcept;
bool operator==(const std::string& str, const ScpiCmdNode& node) noexcept;
bool operator!=(const ScpiCmdNode& node, const std::string& str) noexcept;
bool operator!=(const std::string& str, const ScpiCmdNode& node) noexcept;
