#include "../TEST_DEFINITIONS.h"
#ifdef SCPI_INTERPRETER_TESTS
#pragma once
#include "gtest/gtest.h"
#include "buVariant.h"

std::string testGlobal;
int counter = 0;
class TestA {
	std::string var;
public:
	TestA(std::string&& var) : var(std::move(var)) {}
	TestA() : var("default") {}
	~TestA() {
		testGlobal = var;
		++counter;
	}

	std::string getVar() const {
		return var;
	}
};

class TestB {
public:
	~TestB() {
		testGlobal = "ERROR WRONG TYPE DESTROYED";
	}
};

TEST(buVariant, constructVariant) {
	buVariant<std::string, float, TestA> variant;
	variant.set<TestA>("testing");
	EXPECT_EQ(variant.get<TestA>().getVar(), "testing");
}

TEST(buVariant, destructionProperObjectWhenOutOfLoop) {
	testGlobal = "willChangeWhenVariantWillBeDestructed";
	{
		buVariant<TestA, TestB> variant;
		variant.set<TestA>("GoodDestructorA");
		EXPECT_EQ(testGlobal, "willChangeWhenVariantWillBeDestructed");
	}
	EXPECT_EQ(testGlobal, "GoodDestructorA");

	testGlobal = "willChangeWhenVariantWillBeDestructed";
	{
		buVariant<TestA, TestB> variant;
		variant.set<TestB>();
		EXPECT_EQ(testGlobal, "willChangeWhenVariantWillBeDestructed");
	}
	EXPECT_EQ(testGlobal, "ERROR WRONG TYPE DESTROYED");
}

TEST(buVariant, typeChange) {
	buVariant<std::string, int, TestA> variant;
	variant.set<TestA>("testing");
	EXPECT_EQ(variant.get<TestA>().getVar(), "testing");

	variant.set<int>(22);
	EXPECT_EQ(variant.get<int>(), 22);
}

TEST(buVariant, destructionAfterTypeChange) {
	testGlobal = "willChangeWhenVariantWillBeDestructed";
	buVariant<int, TestA> variant;
	variant.set<TestA>("GoodDestructorA");
	EXPECT_EQ(variant.get<TestA>().getVar(), "GoodDestructorA");

	EXPECT_EQ(testGlobal, "willChangeWhenVariantWillBeDestructed");
	variant.set<int>(22);
	EXPECT_EQ(variant.get<int>(), 22);
	EXPECT_EQ(testGlobal, "GoodDestructorA");
}

TEST(buVariant, allVectorElementsCalledDestructor) {
	{
		buVariant<std::vector<TestA>, int> variant;
		variant.set<std::vector<TestA>>(std::vector<TestA>({ TestA(), TestA() }));
		counter = 0;
	}
	EXPECT_EQ(counter, 2);
}

TEST(buVariant, getBadCastException) {
	buVariant<std::string, int> variant;
	variant.set<int>(15);
	EXPECT_THROW(variant.get<std::string>(), std::bad_cast);
}

TEST(buVariant, setBadTypeId) {
	buVariant<std::string, int> variant;
	EXPECT_THROW(variant.set<unsigned>(15), std::bad_typeid);
}

TEST(buVariant, is) {
	buVariant<std::string, int> variant;
	variant.set<int>(15);
	EXPECT_TRUE(variant.is<int>());
	EXPECT_FALSE(variant.is<std::string>());

	variant.set<std::string>("15");
	EXPECT_FALSE(variant.is<int>());
	EXPECT_TRUE(variant.is<std::string>());
}

TEST(buVariant, valid) {
	buVariant<std::string, int> variant;
	EXPECT_FALSE(variant.valid());
	variant.set<int>(15);
	EXPECT_TRUE(variant.valid());
}

TEST(buVariant, visit) {
	buVariant<std::string, int> variant;
	class visitorTest {
	public:
		void operator()(const std::string& arg) const {
			testGlobal = "string visited";
		}
		void operator()(const int& arg) const {
			testGlobal = "int visited";
		}
	};

	testGlobal = "none";
	variant.set<std::string>("elo");
	EXPECT_EQ(testGlobal, "none");
	visit<visitorTest, std::string, int>(visitorTest{}, variant);
	EXPECT_EQ(testGlobal, "string visited");

	testGlobal = "none";
	variant.set<int>(22);
	EXPECT_EQ(testGlobal, "none");
	visit<visitorTest, std::string, int>(visitorTest{}, variant);
	EXPECT_EQ(testGlobal, "int visited");
}

TEST(buVariant, getTypeId) {
	buVariant<std::string, int> variant;

	variant.set<std::string>("string");
	EXPECT_EQ(variant.getTypeId(), typeid(std::string).hash_code());
	EXPECT_NE(variant.getTypeId(), typeid(int).hash_code());

	variant.set<int>(15);
	EXPECT_NE(variant.getTypeId(), typeid(std::string).hash_code());
	EXPECT_EQ(variant.getTypeId(), typeid(int).hash_code());
}

#endif//#ifdef SCPI_INTERPRETER_TESTS