#pragma once
#include <mutex>
#include "scpiParser.h"
#include "scpiCmdNode.h"

class ScpiExec {
	std::unique_ptr<ScpiCmdNode> cmdTree;
	int errorCode = NO_ERROR;
	void showData(const ParsedScpiData& data) const noexcept;
	bool executeCmd(const ParsedScpiData& data);
public:
	ScpiExec(std::unique_ptr<ScpiCmdNode>&& cmdTree);
	bool parseAndExecuteCommand(const std::string& data);
	int getErrorCode() const {
		return errorCode;
	}
};
