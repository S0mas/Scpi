#include "../TEST_DEFINITIONS.h"
#ifdef SCPI_INTERPRETER_TESTS
#pragma once
#include "gtest/gtest.h"
#include "scpiFunction.h"
#include "typedefs.h"

int foo(const std::vector<ScpiArg>& args) {
	return NO_ERROR;
}

int fooArgsChecking(const std::vector<ScpiArg>& args) {
	if(args[0].get<unsigned long long>() == 16 && args[1].get<signed long long>() == -1999 && args[2].get<std::string>() == "string")
		return 0;
	return 0xFFFF;
}

TEST(scpiFunction, functionCall) {
	auto sf = ScpiFunction::getScpiFunction<>(foo);
	EXPECT_EQ(sf.run({}), 0);
}


TEST(scpiFunction, functionCallWithArgs) {
	auto sf = ScpiFunction::getScpiFunction<unsigned long long, signed long long, std::string>(fooArgsChecking);
	ScpiArg arg1;
	ScpiArg arg2;
	ScpiArg arg3;
	arg1.set<unsigned long long>(16);
	arg2.set<signed long long>(-1999);
	arg3.set<std::string>("string");
	EXPECT_EQ(sf.run({ arg1, arg2, arg3 }), NO_ERROR);
}

TEST(scpiFunction, notEnoughArgsError) {
	auto sf = ScpiFunction::getScpiFunction<unsigned long long, signed long long, std::string, double>(foo);
	ScpiArg arg1;
	ScpiArg arg2;
	ScpiArg arg3;
	arg1.set<unsigned long long>(16);
	arg2.set<signed long long>(-1999);
	arg3.set<std::string>("string");
	EXPECT_EQ(sf.run({ arg1, arg2, arg3 }), ERR_100_MISSING_PARAM);
}

TEST(scpiFunction, tooMuchArgsError) {
	auto sf = ScpiFunction::getScpiFunction<unsigned long long>(foo);
	ScpiArg arg1;
	ScpiArg arg2;
	arg1.set<unsigned long long>(16);
	arg2.set<signed long long>(-1999);
	EXPECT_EQ(sf.run({ arg1, arg2 }), ERR_100_PARAM_NOT_ALLOWED);
}

TEST(scpiFunction, invalidArgsTypesError) {
	auto sf = ScpiFunction::getScpiFunction<unsigned long long, std::string>(foo);
	ScpiArg arg1;
	ScpiArg arg2;
	arg1.set<unsigned long long>(16);
	arg2.set<signed long long>(-1999);
	EXPECT_EQ(sf.run({ arg1, arg2 }), ERR_100_DATA_TYPE);
}
#endif//#ifdef SCPI_INTERPRETER_TESTS