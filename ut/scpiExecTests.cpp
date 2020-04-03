#include "gtest/gtest.h"
#include "../scpiExec.h"
#include "../scpiVisitors.h"

std::string output;
unsigned long long normalParam;
unsigned long long inNameParam1;
unsigned long long inNameParam2;

int foo1(const std::vector<ScpiArg>& args) {
	output = "1";
	return NO_ERROR;
}

int foo2(const std::vector<ScpiArg>& args) {
	output = "2";
	if (args.size() == 2) {
		normalParam = args[0].get<unsigned long long>();
		inNameParam1 = args[1].get<std::vector<unsigned long long>>()[0];
		inNameParam2 = args[1].get<std::vector<unsigned long long>>()[1];
	}

	return NO_ERROR;
}

int foo3(const std::vector<ScpiArg>& args) {
	output = "3";
	return NO_ERROR;
}

TEST(scpiExecTests, parseAndExecute) {
	auto cmdTree = std::make_unique<ScpiCmdNode>("root");
	cmdTree->children.push_back(std::make_unique<ScpiCmdNode>("FIRST", ScpiFunction::getScpiFunction<>(foo1)));
	cmdTree->children.push_back(std::make_unique<ScpiCmdNode>("SECOND", ScpiFunction::getScpiFunction<>(foo2)));
	cmdTree->children.back()->children.push_back(std::make_unique<ScpiCmdNode>("SECOND_DEEPER", ScpiFunction::getScpiFunction<>(foo3)));

	ScpiExec exec(std::move(cmdTree));
	EXPECT_TRUE(exec.parseAndExecuteCommand("FIRST"));
	EXPECT_EQ(output, "1");
	EXPECT_TRUE(exec.parseAndExecuteCommand("SECOND"));
	EXPECT_EQ(output, "2");
	EXPECT_TRUE(exec.parseAndExecuteCommand("SECOND:SECOND_DEEPER"));
	EXPECT_EQ(output, "3");
	EXPECT_FALSE(exec.parseAndExecuteCommand("FIRST:SECOND_DEEPER"));
	EXPECT_FALSE(exec.parseAndExecuteCommand("SECOND_DEEPER"));
}

TEST(scpiExecTests, lastItemsInArgsVectorAreInNameParams) {
	auto cmdTree = std::make_unique<ScpiCmdNode>("root");
	cmdTree->children.push_back(std::make_unique<ScpiCmdNode>("first", ScpiFunction::getScpiFunction<>(foo1), true, 31));
	cmdTree->children.back()->children.push_back(std::make_unique<ScpiCmdNode>("deeper", ScpiFunction::getScpiFunction<unsigned long long, std::vector<unsigned long long>>(foo2), true, 63));

	ScpiExec exec(std::move(cmdTree));
	output = "FAIL";
	normalParam = 0;
	inNameParam1 = 0;
	inNameParam2 = 0;
	EXPECT_TRUE(exec.parseAndExecuteCommand("FIRST22:DEEP44 #h11"));
	std::vector<unsigned long long> inNameParams = { 22, 44 };
	EXPECT_EQ(output, "2");
	EXPECT_EQ(normalParam, 0x11);
	EXPECT_EQ(inNameParam1, inNameParams[0]);
	EXPECT_EQ(inNameParam2, inNameParams[1]);
}

TEST(scpiExecTests, cmdNotFound) {
	auto cmdTree = std::make_unique<ScpiCmdNode>("root");
	cmdTree->children.push_back(std::make_unique<ScpiCmdNode>("FIRST", ScpiFunction::getScpiFunction<>(foo1)));

	ScpiExec exec(std::move(cmdTree));
	EXPECT_EQ(exec.getErrorCode(), NO_ERROR);
	EXPECT_FALSE(exec.parseAndExecuteCommand("DOWN"));
	EXPECT_EQ(exec.getErrorCode(), ERR_100_COMMAND_ERROR);
}
