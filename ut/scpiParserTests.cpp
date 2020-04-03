#include "gtest/gtest.h"
#include "../scpiParser.h"

TEST(ScpiArgsParser, parseHex) {
	EXPECT_EQ(ScpiArgsParser::parseArg("#h0x14").get<unsigned long long>(), 0x14);
	EXPECT_EQ(ScpiArgsParser::parseArg("#hE22").get<unsigned long long>(), 0xE22);
	EXPECT_EQ(ScpiArgsParser::parseArg("#H0xD14").get<unsigned long long>(), 0xD14);
	EXPECT_EQ(ScpiArgsParser::parseArg("#HCaA1").get<unsigned long long>(), 0xCAA1);
}

TEST(ScpiArgsParser, parseOct) {
	EXPECT_EQ(ScpiArgsParser::parseArg("#o777").get<unsigned long long>(), 0777);
	EXPECT_EQ(ScpiArgsParser::parseArg("#O21").get<unsigned long long>(), 021);
	EXPECT_EQ(ScpiArgsParser::parseArg("#o1201").get<unsigned long long>(), 01201);
}

TEST(ScpiArgsParser, parseBin) {
	EXPECT_EQ(ScpiArgsParser::parseArg("#b111").get<unsigned long long>(), 0b111);
	EXPECT_EQ(ScpiArgsParser::parseArg("#B1111111110011").get<unsigned long long>(), 0b1111111110011);
}

TEST(ScpiArgsParser, parseSignedLL) {
	//EXPECT_EQ(ScpiArgsParser::parseArg("-111").get<signed long long>(), -111);
}

TEST(ScpiArgsParser, parseDouble) {
	EXPECT_EQ(ScpiArgsParser::parseArg("-111.12").get<double>(), -111.12);
	EXPECT_EQ(ScpiArgsParser::parseArg("111.12").get<double>(), 111.12);
	EXPECT_EQ(ScpiArgsParser::parseArg("+111.12").get<double>(), 111.12);
	EXPECT_EQ(ScpiArgsParser::parseArg("-111").get<double>(), -111);
	EXPECT_EQ(ScpiArgsParser::parseArg("111.11e+02").get<double>(), 11111);
	EXPECT_EQ(ScpiArgsParser::parseArg("1.0006E3").get<double>(), 1000.6);
	EXPECT_EQ(ScpiArgsParser::parseArg("111.11E-02").get<double>(), 1.1111);
	EXPECT_EQ(ScpiArgsParser::parseArg("1.0006e-3").get<double>(), 0.0010006);
	EXPECT_EQ(ScpiArgsParser::parseArg(".011").get<double>(), .011);
	EXPECT_EQ(ScpiArgsParser::parseArg("-.044").get<double>(), -.044);
}

TEST(ScpiArgsParser, parseString) {
	EXPECT_EQ(ScpiArgsParser::parseArg("'testStr'").get<std::string>(), "testStr");
	EXPECT_EQ(ScpiArgsParser::parseArg("\"testStr2\"").get<std::string>(), "testStr2");
	EXPECT_EQ(ScpiArgsParser::parseArg("' test string '").get<std::string>(), " test string ");
	EXPECT_EQ(ScpiArgsParser::parseArg("',test,str,i  ,  ng ,'").get<std::string>(), ",test,str,i  ,  ng ,");
	EXPECT_EQ(ScpiArgsParser::parseArg("\",test,str,i  ,  ng ,\"").get<std::string>(), ",test,str,i  ,  ng ,");
	EXPECT_EQ(ScpiArgsParser::parseArg("\",test,str,i  ,  ng ,\"").get<std::string>(), ",test,str,i  ,  ng ,");
	EXPECT_EQ(ScpiArgsParser::parseArg("'''\"''").get<std::string>(), "''\"'");
}

TEST(ScpiArgsParser, parseList) {
	auto list = ScpiArgsParser::parseArg("(@1)").get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 1);
	EXPECT_EQ(list.front(), 1);

	list = ScpiArgsParser::parseArg("(@1:2)").get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list.front(), 1);
	EXPECT_EQ(list.back(), 2);

	list = ScpiArgsParser::parseArg("(@2:1)").get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list.front(), 2);
	EXPECT_EQ(list.back(), 1);

	list = ScpiArgsParser::parseArg("(@1,5,2:3)").get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 4);
	EXPECT_EQ(list.front(), 1);
	EXPECT_EQ(list[1], 5);
	EXPECT_EQ(list[2], 2);
	EXPECT_EQ(list.back(), 3);

	list = ScpiArgsParser::parseArg("(@2:3,1,5)").get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 4);
	EXPECT_EQ(list.front(), 2);
	EXPECT_EQ(list[1], 3);
	EXPECT_EQ(list[2], 1);
	EXPECT_EQ(list.back(), 5);
}

TEST(ScpiArgsParser, throwEmptyArgData) {
	EXPECT_THROW(ScpiArgsParser::parseArg(""), ScpiArgsParser::EmptyArgDataException);
}

TEST(ScpiArgsParser, throwInvalidArgData) {
	EXPECT_THROW(ScpiArgsParser::parseArg("#d111"), ScpiArgsParser::InvalidArgDataException);
	EXPECT_THROW(ScpiArgsParser::parseArg("#o+"), ScpiArgsParser::InvalidArgDataException);
	EXPECT_THROW(ScpiArgsParser::parseArg("#b"), ScpiArgsParser::InvalidArgDataException);
	EXPECT_THROW(ScpiArgsParser::parseArg("#b"), ScpiArgsParser::InvalidArgDataException);
}

TEST(ScpiArgsParser, parseMnemonic) {
	//TODO
}

TEST(ScpiParser, parseFlatCmdWithNoArgs) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 0);
}

TEST(ScpiParser, parseFlatCmdWithLeadingColon) {
	auto parsedData = ScpiParser::parse(":FIRST_CMD_NODE");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 0);
}

TEST(ScpiParser, parseDeepCmdWithNoArgs) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE:SECOND:LAST");
	EXPECT_EQ(parsedData.nodesNames.size(), 3);
	EXPECT_EQ(parsedData.nodesNames[0], "FIRST_CMD_NODE");
	EXPECT_EQ(parsedData.nodesNames[1], "SECOND");
	EXPECT_EQ(parsedData.nodesNames.back(), "LAST");

	EXPECT_EQ(parsedData.args.size(), 0);
}

TEST(ScpiParser, parseFlatCmdWith1Args) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE 'arg'");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 1);
	EXPECT_EQ(parsedData.args.back().get<std::string>(), "arg");
}

TEST(ScpiParser, parseFlatCmdWith3Args) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE #h0x11, +15.2, -444");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<unsigned long long>(), 0x11);
	EXPECT_EQ(parsedData.args[1].get<double>(), 15.2);
	EXPECT_EQ(parsedData.args.back().get<double>(), -444);
}

TEST(ScpiParser, parseFlatCmdWith3StringArgsWithSpecialChars) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE '  ,,  ', 'abc  , dab,  ,', ', j j ,'");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<std::string>(), "  ,,  ");
	EXPECT_EQ(parsedData.args[1].get<std::string>(), "abc  , dab,  ,");
	EXPECT_EQ(parsedData.args[2].get<std::string>(), ", j j ,");
}

TEST(ScpiParser, parseFlatCmdWith3ArgsAndExtraSpaces) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE    #h0x11,+15.2,      -444   ");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<unsigned long long>(), 0x11);
	EXPECT_EQ(parsedData.args[1].get<double>(), 15.2);
	EXPECT_EQ(parsedData.args.back().get<double>(), -444);
}

TEST(ScpiParser, parseDeepCmdWith3ArgsAndExtraSpaces) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE:SECOND:LAST    #b10011,-15.2,      444   ");
	EXPECT_EQ(parsedData.nodesNames.size(), 3);
	EXPECT_EQ(parsedData.nodesNames[0], "FIRST_CMD_NODE");
	EXPECT_EQ(parsedData.nodesNames[1], "SECOND");
	EXPECT_EQ(parsedData.nodesNames.back(), "LAST");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<unsigned long long>(), 0b10011);
	EXPECT_EQ(parsedData.args[1].get<double>(), -15.2);
	EXPECT_EQ(parsedData.args.back().get<double>(), 444);
}

TEST(ScpiParser, parseDeepCmdWith3ArgsAndExtraSpacesAndLeadingColon) {
	auto parsedData = ScpiParser::parse(":FIRST_CMD_NODE:SECOND:LAST    #b10011,-15.2,      444   ");
	EXPECT_EQ(parsedData.nodesNames.size(), 3);
	EXPECT_EQ(parsedData.nodesNames[0], "FIRST_CMD_NODE");
	EXPECT_EQ(parsedData.nodesNames[1], "SECOND");
	EXPECT_EQ(parsedData.nodesNames.back(), "LAST");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<unsigned long long>(), 0b10011);
	EXPECT_EQ(parsedData.args[1].get<double>(), -15.2);
	EXPECT_EQ(parsedData.args.back().get<double>(), 444);
}

TEST(ScpiParser, parseFlatCmdWith3ArgsOneIsList) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE #h0x11, (@2:3,6,1), -444");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<unsigned long long>(), 0x11);
	auto list = parsedData.args[1].get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 4);
	EXPECT_EQ(list.front(), 2);
	EXPECT_EQ(list[1], 3);
	EXPECT_EQ(list[2], 6);
	EXPECT_EQ(list.back(), 1);
	EXPECT_EQ(parsedData.args.back().get<double>(), -444);
}

TEST(ScpiParser, parseFlatCmdWith3ArgsTwoAreLists) {
	auto parsedData = ScpiParser::parse("FIRST_CMD_NODE #h0x11, (@2:3,6,1), (@3:2,4,5)");
	EXPECT_EQ(parsedData.nodesNames.size(), 1);
	EXPECT_EQ(parsedData.nodesNames.back(), "FIRST_CMD_NODE");

	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<unsigned long long>(), 0x11);
	auto list = parsedData.args[1].get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 4);
	EXPECT_EQ(list.front(), 2);
	EXPECT_EQ(list[1], 3);
	EXPECT_EQ(list[2], 6);
	EXPECT_EQ(list.back(), 1);
	list = parsedData.args[2].get<std::vector<unsigned long long>>();
	EXPECT_EQ(list.size(), 4);
	EXPECT_EQ(list.front(), 3);
	EXPECT_EQ(list[1], 2);
	EXPECT_EQ(list[2], 4);
	EXPECT_EQ(list.back(), 5);
}

TEST(ScpiParser, parseDeeperCmdWith3ArgsOneAreList) {
	auto parsedData = ScpiParser::parse("INPUT:EXCITATION 20.24, -20.45, (@0:6)");
	EXPECT_EQ(parsedData.nodesNames.size(), 2);
	EXPECT_EQ(parsedData.nodesNames[0], "INPUT");
	EXPECT_EQ(parsedData.nodesNames.back(), "EXCITATION");
	EXPECT_EQ(parsedData.args.size(), 3);
	EXPECT_EQ(parsedData.args[0].get<double>(), 20.24);
	EXPECT_EQ(parsedData.args[1].get<double>(), -20.45);
	EXPECT_EQ(parsedData.args[2].get<std::vector<unsigned long long>>(), std::vector<unsigned long long>({0, 1, 2, 3, 4, 5 ,6 }));
	
}