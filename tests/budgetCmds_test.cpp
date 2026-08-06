#include <filesystem>
#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/otherCmds.hpp"
#include "core/database.hpp"
#include "core/path.hpp"

namespace fs = std::filesystem;

class BudgetCmdsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Start every test with a completely fresh database.
        fs::remove(PATH / "budget-data.db");

        ASSERT_EQ(cmdInit(), 0);

        const char* argv[] = {
            "TEST",
            "2026-01-01",
            "2026-08-02",
            "1000"
        };

        ASSERT_EQ(budgetAdd(4, argv), 0);
    }

    void TearDown() override {
        Database db(PATH / "budget-data.db");

        auto current = db.getSetting("current_budget");
        if (current) {
            db.deleteBudget(std::stoi(*current));
        }

        fs::remove(PATH / "budget-data.db");
    }
};


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidName1) {
    const char* argv[] = {
        "_testbudget-s",
        "2026-01-01",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(4, argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidName2) {
    const char* argv[] = {
        "0aBcD123!",
        "2026-01-01",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(4, argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidStartDate1) {
    const char* argv[] = {
        "TEST2",
        "banana",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(4, argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidStartDate2) {
    const char* argv[] = {
        "TEST2",
        "2027-01-01",
        "2026-12-31",
        "1000"
    };

    EXPECT_EQ(budgetAdd(4, argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidEndDate1) {
    const char* argv[] = {
        "TEST2",
        "2026-01-01",
        "dog and cat",
        "1000"
    };

    EXPECT_EQ(budgetAdd(4, argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetAddRejectInvalidEndDate2) {
    const char* argv[] = {
        "TEST2",
        "2026-01-01",
        "01-01-2026",
        "1000"
    };

    EXPECT_EQ(budgetAdd(4, argv), 1);
}


TEST_F(BudgetCmdsTest, BudgetEditSuccess) {
    const char* args1[] = {
        "start_date",
        "2025-01-21"
    };

    EXPECT_EQ(budgetEdit(2, args1), 0);

    const char* args2[] = {
        "end_date",
        "2027-04-04"
    };

    EXPECT_EQ(budgetEdit(2, args2), 0);
}


TEST_F(BudgetCmdsTest, BudgetEditInvalidName) {
    const char* args[] = {
        "name",
        "_test"
    };

    EXPECT_EQ(budgetEdit(2, args), 1);
}


TEST_F(BudgetCmdsTest, BudgetEditInvalidStartDate) {
    const char* args[] = {
        "start_date",
        "1/1/2026"
    };

    EXPECT_EQ(budgetEdit(2, args), 1);
}


TEST_F(BudgetCmdsTest, BudgetEditInvalidEndDate) {
    const char* args[] = {
        "end_date",
        "25-25-25"
    };

    EXPECT_EQ(budgetEdit(2, args), 1);
}


TEST_F(BudgetCmdsTest, BudgetEditInvalidLimit) {
    const char* args[] = {
        "limit",
        "abc123"
    };

    EXPECT_EQ(budgetEdit(2, args), 1);
}


TEST_F(BudgetCmdsTest, BudgetListSuccess) {
    EXPECT_EQ(budgetList(), 0);
}
