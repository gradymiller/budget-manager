#include <gtest/gtest.h>

#include <filesystem>

#include "core/path.hpp"
#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"
#include "cli/transactionCmds.hpp"

class EdgeCaseTest : public ::testing::Test {
protected:
    void SetUp() override {
		std::filesystem::remove(PATH / "budget-data.db");

		cmdInit();

        const char* budget[] = {
            "TEST",
            "2026-01-01",
            "2026-12-31"
        };

        ASSERT_EQ(budgetAdd(3, budget), 0);

        const char* current[] = {
            "1"
        };

        ASSERT_EQ(cmdSwitch(1, current), 0);

        const char* category[] = {
            "Food",
            "expense",
            "500"
        };

        ASSERT_EQ(categoryAdd(3, category), 0);
    }

    void TearDown() override {
        const char* budget[] = {"TEST"};
        budgetDelete(1, budget);

        const char* budget2[] = {"SECOND"};
        budgetDelete(1, budget2);
    }
};

// ---------------------------------------------------------
// Budget edge cases
// ---------------------------------------------------------

TEST_F(EdgeCaseTest, BudgetAddRejectInvalidCalendarDate) {
    const char* args[] = {
        "TEST2",
        "2026-02-31",
        "2026-12-31"
    };

    EXPECT_EQ(budgetAdd(3, args), 1);
}

// ---------------------------------------------------------
// Category edge cases
// ---------------------------------------------------------

TEST_F(EdgeCaseTest, CategoryRejectEmptyName) {
    const char* args[] = {
        "",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, args), 1);
}

TEST_F(EdgeCaseTest, CategoryRejectNegativeLimit) {
    const char* args[] = {
        "Negative",
        "expense",
        "-100"
    };

    EXPECT_EQ(categoryAdd(3, args), 1);
}

TEST_F(EdgeCaseTest, CategoryDuplicateBehavior) {
    const char* args[] = {
        "Food",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, args), 0);
}

TEST_F(EdgeCaseTest, CategoryEditRejectUnknownField) {
    const char* args[] = {
        "1",
        "unknown",
        "value"
    };

    EXPECT_EQ(categoryEdit(3, args), 1);
}

// ---------------------------------------------------------
// Transaction edge cases
// ---------------------------------------------------------

TEST_F(EdgeCaseTest, TransactionRejectNegativeAmount) {
    const char* args[] = {
        "-50",
        "1",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, args), 1);
}

TEST_F(EdgeCaseTest, TransactionRejectZeroAmount) {
    const char* args[] = {
        "0",
        "1",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, args), 1);
}

TEST_F(EdgeCaseTest, TransactionRejectMissingDateValue) {
    const char* args[] = {
        "50",
        "1",
        "expense",
        "--date"
    };

    EXPECT_EQ(transactionAdd(4, args), 1);
}

TEST_F(EdgeCaseTest, TransactionRejectMissingVendorValue) {
    const char* args[] = {
        "50",
        "1",
        "expense",
        "--vendor"
    };

    EXPECT_EQ(transactionAdd(4, args), 1);
}

TEST_F(EdgeCaseTest, TransactionRejectInvalidID) {
    const char* args[] = {
        "-1",
        "amount",
        "50"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}

TEST_F(EdgeCaseTest, TransactionEditCategory) {
    const char* add[] = {
        "50",
        "1",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, add), 0);

    const char* category2[] = {
        "Gas",
        "expense",
        "200"
    };

    ASSERT_EQ(categoryAdd(3, category2), 0);

    const char* edit[] = {
        "1",
        "category",
        "2"
    };

    EXPECT_EQ(transactionEdit(3, edit), 0);
}

// ---------------------------------------------------------
// Multiple budget state tests
// ---------------------------------------------------------

TEST_F(EdgeCaseTest, MultipleBudgetsCanExist) {
    const char* args[] = {
        "SECOND",
        "2026-01-01",
        "2026-12-31"
    };

    EXPECT_EQ(budgetAdd(4, args), 0);
}

TEST_F(EdgeCaseTest, SwitchRejectsMissingBudget) {
    const char* args[] = {
        "DOES_NOT_EXIST"
    };

    EXPECT_EQ(cmdSwitch(1, args), 1);
}
