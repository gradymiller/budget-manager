#include <gtest/gtest.h>
#include <filesystem>
#include "cli/budgetCmds.hpp"
#include "core/path.hpp"

class BudgetCmdsTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::filesystem::remove(PATH / "TEST.csv");
    }
};

const char* const* budgetHelper() {
    static const char* argv[] = {
        "TEST",
        "2026-01-01",
        "2026-08-02",
        "1000"
    };

    return argv;
}

TEST_F(BudgetCmdsTest, BudgetAddSuccess) {
    const char* const* argv = budgetHelper();

    EXPECT_EQ(budgetAdd(argv), 0);
    EXPECT_TRUE(std::filesystem::exists(PATH / "TEST.csv"));
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidName1) {
    const char* argv[] = {
        "_testbudget-s",
        "2026-01-01",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidName2) {
    const char* argv[] = {
        "0aBcD123!",
        "2026-01-01",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidLimit) {
    const char* argv[] = {
        "TEST",
        "2026-02-02",
        "2026-12-31",
        "abc"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidStartDate1) {
    const char* argv[] = {
        "TEST",
        "banana",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidStartDate2) {
    const char* argv[] = {
        "TEST",
        "2027-01-01",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidEndDate1) {
    const char* argv[] = {
        "TEST",
        "2027-01-01",
        "dog and cat",
        "1000"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidEndDate2) {
    const char* argv[] = {
        "TEST",
        "2027-01-01",
        "01-01-2026",
        "1000"
    };

    EXPECT_EQ(budgetAdd(argv), 1);
}

TEST_F(BudgetCmdsTest, BudgetEditSuccess) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    const char* args1[] = {"name", "test"};
    EXPECT_EQ(budgetEdit(args1), 0);

    const char* args2[] = {"start_date", "2025-01-21"};
    EXPECT_EQ(budgetEdit(args2), 0);

    const char* args3[] = {"end_date", "2027-04-04"};
    EXPECT_EQ(budgetEdit(args3), 0);

    const char* args4[] = {"limit", "12345.55"};
    EXPECT_EQ(budgetEdit(args4), 0);
}

TEST_F(BudgetCmdsTest, BudgetEditInvalidName) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    const char* args[] = {"name", "_test"};

    EXPECT_EQ(budgetEdit(args), 1);
}

TEST_F(BudgetCmdsTest, BudgetEditInvalidStartDate) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    const char* args[] = {"start_date", "1/1/2026"};

    EXPECT_EQ(budgetEdit(args), 1);
}

TEST_F(BudgetCmdsTest, BudgetEditInvalidEndDate) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    const char* args[] = {"end_date", "25-25-25"};

    EXPECT_EQ(budgetEdit(args), 1);
}

TEST_F(BudgetCmdsTest, BudgetEditInvalidLimit) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    const char* args[] = {"limit", "abc123"};

    EXPECT_EQ(budgetEdit(args), 1);
}

TEST_F(BudgetCmdsTest, BudgetDeleteSuccess) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    EXPECT_EQ(budgetDelete(argv), 0);
    EXPECT_FALSE(std::filesystem::exists(PATH / "TEST.csv"));
}

TEST_F(BudgetCmdsTest, BudgetDeleteInvalidName) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    const char* args[] = {"THIS_BUDGET_SHOULD_NOT_EXIST"};

    EXPECT_EQ(budgetDelete(args), 1);
}

TEST_F(BudgetCmdsTest, BudgetListSuccess) {
    const char* const* argv = budgetHelper();
    budgetAdd(argv);

    EXPECT_EQ(budgetList(), 0);
}
