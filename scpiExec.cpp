#include "scpiExec.h"
#include "scpiVisitors.h"
#include <iostream>

void ScpiExec::showData(const ParsedScpiData& data) const noexcept {
	for (auto const& name : data.nodesNames)
		std::cout << name << " ";
	for (auto const& arg : data.args)
		visit<ScpiArgVisitor, std::string, unsigned long long, signed long long, double, std::vector<unsigned long long>>(ScpiArgVisitor{}, arg);
	std::cout << std::endl;
}

bool ScpiExec::executeCmd(const ParsedScpiData& data) {
	showData(data);
	auto node = cmdTree->findCmd(data.nodesNames);
	if (!node || !node->isFunctionDefined()) {
		errorCode = ERR_100_COMMAND_ERROR;
		return false;
	}
	errorCode = node->run(data.args);

	return errorCode == NO_ERROR;
}

ScpiExec::ScpiExec(std::unique_ptr<ScpiCmdNode>&& cmdTree) : cmdTree(std::move(cmdTree)) {}

bool ScpiExec::parseAndExecuteCommand(const std::string& data) {
	try {
		return executeCmd(ScpiParser::parse(data));
	}
	catch (...) {
		std::cout << "\nPARSE ERROR --> DBUG ONLY\n";
		return false;
	}
}