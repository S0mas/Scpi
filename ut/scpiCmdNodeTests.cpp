#include "gtest/gtest.h"
#include "../scpiCmdNode.h"
#include <iostream>

static int bar1(const std::vector<ScpiArg>& args) {
	return 1;
}

static int bar2(const std::vector<ScpiArg>& args) {
	return 2;
}
TEST(ScpiCmdNodeTests, emptyCmdName) {
	std::string name = "";
	ScpiCmdNode node(name);
	EXPECT_EQ(node.getFullName(), name);
}
TEST(ScpiCmdNodeTests, fullName) {
	std::string name = "foo";
	ScpiCmdNode node(name);
	EXPECT_EQ(node.getFullName(), name);
}
TEST(ScpiCmdNodeTests, shortName) {
	std::string name = "foo";
	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), name);
}
TEST(ScpiCmdNodeTests, run1) {
	std::string name = "foo";
	ScpiCmdNode node(name, ScpiFunction::getScpiFunction<>(bar1));
	EXPECT_EQ(node.run({}), 1);
}
TEST(ScpiCmdNodeTests, run2) {
	std::string name = "foo";
	ScpiCmdNode node(name, ScpiFunction::getScpiFunction<>(bar2));
	EXPECT_EQ(node.run({}), 2);
}
TEST(ScpiCmdNodeTests, operatorEq) {
	std::string name = "SHORTname";
	ScpiCmdNode node(name);
	EXPECT_TRUE(node == name);
	EXPECT_TRUE(node == std::string("shor"));
	EXPECT_TRUE(node == std::string("sHoR"));
	EXPECT_TRUE(node == std::string("sHoRTNAME"));
	EXPECT_TRUE(node == std::string("sHoRTnamE"));

	EXPECT_FALSE(node == std::string("shortna"));
	EXPECT_FALSE(node == std::string("sHoRT"));
	EXPECT_FALSE(node == std::string("sHoRNAME"));
	EXPECT_FALSE(node == std::string("sHoTnamEE"));

	EXPECT_TRUE(name == node);
	EXPECT_FALSE(std::string("shortna") == node);
}
TEST(ScpiCmdNodeTests, shortNameCreator_size4) {
	std::string name = "SHOR";

	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), "shor");
}
TEST(ScpiCmdNodeTests, shortNameCreator_size5LastNotVowel) {
	std::string name = "SHORT";

	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), "shor");
}
TEST(ScpiCmdNodeTests, shortNameCreator_size5LastVowel) {
	std::string name = "SHOUT";

	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), "sho");
}
TEST(ScpiCmdNodeTests, shortNameCreator_size4Query) {
	std::string name = "SHOR?";

	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), "shor?");
}
TEST(ScpiCmdNodeTests, shortNameCreator_size5LastNotVowelQuery) {
	std::string name = "SHORT?";

	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), "shor?");
}
TEST(ScpiCmdNodeTests, shortNameCreator_size5LastVowelQuery) {
	std::string name = "SHOUT?";

	ScpiCmdNode node(name);
	EXPECT_EQ(node.getShortName(), "sho?");
}
TEST(ScpiCmdNodeTests, findCmd) {
	std::string name = "ROOT";

	ScpiCmdNode node(name);
	auto nodeFirst = std::make_unique<ScpiCmdNode>("first", ScpiFunction::getScpiFunction<>(bar1));
	auto nodeDeeper = std::make_unique<ScpiCmdNode>("deeper", ScpiFunction::getScpiFunction<>(bar2));
	auto nodeDeeper2 = std::make_unique<ScpiCmdNode>("Dsecond", ScpiFunction::getScpiFunction<>(bar2));

	auto ptrNodeFirst = nodeFirst.get();
	auto ptrNodeDeeper = nodeDeeper.get();
	auto ptrNodeDeeper2 = nodeDeeper2.get();
	
	node.children.push_back(std::move(nodeFirst));
	node.children.back()->children.push_back(std::move(nodeDeeper));
	node.children.back()->children.push_back(std::move(nodeDeeper2));

	EXPECT_EQ(node.findCmd({std::string("first"), std::string("deeper") }), ptrNodeDeeper);
	EXPECT_EQ(node.findCmd({std::string("first"), std::string("Dsecond") }), ptrNodeDeeper2);
	EXPECT_EQ(node.findCmd(std::vector<std::string>({"first"})), ptrNodeFirst);
	EXPECT_EQ(node.findCmd(std::vector<std::string>({"deeper"})), nullptr);
	EXPECT_EQ(node.findCmd({std::string("first"), std::string("deeper"), std::string("deeper")}), nullptr);
	
}
TEST(ScpiCmdNodeTests, operatorEqForNodeWithInNameParam) {
	ScpiCmdNode node("SHORTname", ScpiFunction::getScpiFunction<>(), true, 16);
	EXPECT_TRUE(node == "SHORTname15");
	EXPECT_TRUE(node == "SHORTname1");
	EXPECT_TRUE(node == "SHORTname2");

	EXPECT_FALSE(node == "SHORTname17");
	EXPECT_FALSE(node == "SHORTname0");
	EXPECT_FALSE(node == "SHORTname01");
	EXPECT_FALSE(node == "SHORTname015");
}
TEST(ScpiCmdNodeTests, findCmdWithInNameParam) {
	std::string name = "ROOT";

	ScpiCmdNode node(name);
	auto nodeFirst = std::make_unique<ScpiCmdNode>("first", ScpiFunction::getScpiFunction<>(bar1));
	auto nodeDeeper = std::make_unique<ScpiCmdNode>("deeper", ScpiFunction::getScpiFunction<>(bar2), true, 63);
	auto nodeDeeper2 = std::make_unique<ScpiCmdNode>("Dsecond", ScpiFunction::getScpiFunction<>(bar2));

	auto ptrNodeFirst = nodeFirst.get();
	auto ptrNodeDeeper = nodeDeeper.get();
	auto ptrNodeDeeper2 = nodeDeeper2.get();

	node.children.push_back(std::move(nodeFirst));
	node.children.back()->children.push_back(std::move(nodeDeeper));
	node.children.back()->children.push_back(std::move(nodeDeeper2));

	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper22") }), ptrNodeDeeper);
	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper11") }), ptrNodeDeeper);
	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper63") }), ptrNodeDeeper);
	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper22") }), ptrNodeDeeper);

	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper0") }), nullptr);
	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper64") }), nullptr);
	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper063") }), nullptr);
	EXPECT_EQ(node.findCmd({ std::string("first"), std::string("deeper01") }), nullptr);
}
