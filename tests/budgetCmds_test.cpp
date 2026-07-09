#include <gtest/gtest.h>
#include "cli/budgetCmds.hpp"
#include "core/path.hpp"


class BudgetCmdsTest : public ::testing::Test {
	protected:
		void TearDown() override {
			std::filesystem::remove(PATH / "TEST.csv");
		}
};

TEST_F(BudgetCmdsTest, BudgetAddSuccess) {
	char name[] = "TEST";
	char start[] = "2026-01-01";
	char end[] = "2026-12-31";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 0);
	EXPECT_TRUE(std::filesystem::exists(PATH / "TEST.csv"));
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidName1) {
	char name[] = "_testbudget-s";
	char start[] = "2026-01-01";
	char end[] = "2026-12-31";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidName2) {
	char name[] = "0aBcD123!";
	char start[] = "2026-01-01";
	char end[] = "2026-12-31";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidLimit) {
	char name[] = "TEST";
	char start[] = "2026-02-02";
	char end[] = "2026-12-31";
	char limit[] = "abc";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidStartDate1) {
	char name[] = "TEST";
	char start[] = "banana";
	char end[] = "2026-12-31";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidStartDate2) {
	char name[] = "TEST";
	char start[] = "2027-01-01";
	char end[] = "2026-12-31";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidEndDate1) {
	char name[] = "TEST";
	char start[] = "2027-01-01";
	char end[] = "dog and cat";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidEndDate2) {
	char name[] = "TEST";
	char start[] = "2027-01-01";
	char end[] = "01-01-2026";
	char limit[] = "1000";

	char* argv[] = {name, start, end, limit};

	EXPECT_EQ(budgetAdd(argv), 1);
}
