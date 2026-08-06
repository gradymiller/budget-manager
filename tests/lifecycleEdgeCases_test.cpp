#include <gtest/gtest.h>
#include <filesystem>

#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/transactionCmds.hpp"
#include "cli/otherCmds.hpp"

#include "core/budget.hpp"
#include "core/database.hpp"
#include "core/path.hpp"


class LifecycleEdgeCaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove(PATH / "budget-data.db");

        cmdInit();

        const char* budgetArgs[] = {
            "TEST",
            "2026-01-01",
            "2026-12-31"
        };

        ASSERT_EQ(budgetAdd(4, budgetArgs), 0);

        const char* switchArgs[] = {
            "1"
        };

        ASSERT_EQ(cmdSwitch(1, switchArgs), 0);

        const char* categoryArgs[] = {
            "test-category",
            "expense",
            "1000"
        };

        ASSERT_EQ(categoryAdd(3, categoryArgs), 0);
    }

    void TearDown() override {
        const char* cleanup1[] = {"TEST"};
        budgetDelete(1, cleanup1);

        const char* cleanup2[] = {"SECOND"};
        budgetDelete(1, cleanup2);
    }
};


// ---------------------------------------------------------
// Transaction persistence
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, TransactionPersistsAfterReload)
{
    const char* args[] = {
        "75",
        "1",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, args), 0);

    Database db(PATH / "budget-data.db");
    Budget budget = db.loadBudget();

    EXPECT_EQ(budget.getTransactions().size(), 1);
}


TEST_F(LifecycleEdgeCaseTest, MultipleTransactionsHaveUniqueIDs)
{
    const char* txn1[] = {
        "10",
        "1",
        "expense"
    };

    const char* txn2[] = {
        "20",
        "1",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, txn1), 0);
    ASSERT_EQ(transactionAdd(3, txn2), 0);

    Database db(PATH / "budget-data.db");
    Budget budget = db.loadBudget();

    EXPECT_EQ(budget.getTransactions().size(), 2);
}


TEST_F(LifecycleEdgeCaseTest, DeleteTransactionOnlyDeletesTarget)
{
    const char* txn1[] = {
        "10",
        "1",
        "expense"
    };

    const char* txn2[] = {
        "20",
        "1",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, txn1), 0);
    ASSERT_EQ(transactionAdd(3, txn2), 0);

    const char* deleteArgs[] = {
        "1"
    };

    ASSERT_EQ(transactionDelete(1, deleteArgs), 0);

    Database db(PATH / "budget-data.db");
    Budget budget = db.loadBudget();

    EXPECT_EQ(budget.getTransactions().size(), 1);
}


// ---------------------------------------------------------
// Multiple budget isolation
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, BudgetsHaveSeparateTransactions)
{
    const char* secondBudget[] = {
        "SECOND",
        "2026-01-01",
        "2026-12-31"
    };

    ASSERT_EQ(budgetAdd(4, secondBudget), 0);

    const char* txn[] = {
        "100",
        "1",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, txn), 0);

    const char* switchSecond[] = {
        "2"
    };

    ASSERT_EQ(cmdSwitch(1, switchSecond), 0);

    Database db(PATH / "budget-data.db");
    Budget budget = db.loadBudget();

    EXPECT_EQ(budget.getTransactions().size(), 0);
}


TEST_F(LifecycleEdgeCaseTest, MultipleBudgetsCanBeSwitched)
{
    const char* secondBudget[] = {
        "SECOND",
        "2026-01-01",
        "2026-12-31"
    };

    ASSERT_EQ(budgetAdd(4, secondBudget), 0);

    const char* switchSecond[] = {
        "2"
    };

    EXPECT_EQ(cmdSwitch(1, switchSecond), 0);

    const char* switchFirst[] = {
        "1"
    };

    EXPECT_EQ(cmdSwitch(1, switchFirst), 0);
}


// ---------------------------------------------------------
// Budget rename lifecycle
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, RenamingBudgetPreservesMetadata)
{
    const char* edit[] = {
        "name",
        "RENAMED"
    };

    EXPECT_EQ(budgetEdit(2, edit), 0);

    const char* switchArgs[] = {
        "1"
    };

    EXPECT_EQ(cmdSwitch(1, switchArgs), 0);
}


TEST_F(LifecycleEdgeCaseTest, RenamingBudgetPreservesTransactions)
{
    const char* txn[] = {
        "50",
        "1",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, txn), 0);

    const char* edit[] = {
        "name",
        "RENAMED"
    };

    ASSERT_EQ(budgetEdit(2, edit), 0);

    const char* switchArgs[] = {
        "1"
    };

    ASSERT_EQ(cmdSwitch(1, switchArgs), 0);

    Database db(PATH / "budget-data.db");
    Budget budget = db.loadBudget();

    EXPECT_EQ(budget.getTransactions().size(), 1);
}


// ---------------------------------------------------------
// Numeric boundaries
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, TransactionSupportsDecimalValues)
{
    const char* args[] = {
        "10.99",
        "1",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, args), 0);
}
