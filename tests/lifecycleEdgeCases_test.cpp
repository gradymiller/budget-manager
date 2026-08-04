#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/transactionCmds.hpp"
#include "cli/otherCmds.hpp"

#include "core/budget.hpp"
#include "core/path.hpp"


class LifecycleEdgeCaseTest : public ::testing::Test {
protected:

    void SetUp() override {
        try {
            cmdInit();

            const char* cleanup[] = {"TEST"};
            budgetDelete(1, cleanup);

            const char* cleanup2[] = {"SECOND"};
            budgetDelete(1, cleanup2);

        } catch (...) {}


        const char* budgetArgs[] = {
            "TEST",
            "2026-01-01",
            "2026-12-31",
            "1000"
        };

        ASSERT_EQ(budgetAdd(4, budgetArgs), 0);

        const char* switchArgs[] = {
            "TEST"
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

        const char* cleanup[] = {"TEST"};
        budgetDelete(1, cleanup);

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
        "test-category",
        "expense"
    };

    ASSERT_EQ(transactionAdd(3, args), 0);


    Budget budget;

    auto transactions = budget.getTransactions();

    EXPECT_EQ(transactions.size(), 1);
}



TEST_F(LifecycleEdgeCaseTest, MultipleTransactionsHaveUniqueIDs)
{
    const char* txn1[] = {
        "10",
        "test-category",
        "expense"
    };

    const char* txn2[] = {
        "20",
        "test-category",
        "expense"
    };


    ASSERT_EQ(transactionAdd(3, txn1), 0);
    ASSERT_EQ(transactionAdd(3, txn2), 0);


    Budget budget;


    auto transactions = budget.getTransactions();

    EXPECT_EQ(transactions.size(), 2);
}



TEST_F(LifecycleEdgeCaseTest, DeleteTransactionOnlyDeletesTarget)
{
    const char* txn1[] = {
        "10",
        "test-category",
        "expense"
    };

    const char* txn2[] = {
        "20",
        "test-category",
        "expense"
    };


    ASSERT_EQ(transactionAdd(3, txn1), 0);
    ASSERT_EQ(transactionAdd(3, txn2), 0);


    const char* deleteArgs[] = {
        "0"
    };


    ASSERT_EQ(transactionDelete(1, deleteArgs), 0);


    Budget budget;

    EXPECT_EQ(
        budget.getTransactions().size(),
        1
    );
}



// ---------------------------------------------------------
// Multiple budget isolation
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, BudgetsHaveSeparateTransactions)
{
    const char* secondBudget[] = {
        "SECOND",
        "2026-01-01",
        "2026-12-31",
        "5000"
    };


    ASSERT_EQ(budgetAdd(4, secondBudget), 0);


    const char* txn[] = {
        "100",
        "test-category",
        "expense"
    };


    ASSERT_EQ(transactionAdd(3, txn), 0);



    const char* switchSecond[] = {
        "SECOND"
    };


    ASSERT_EQ(cmdSwitch(1, switchSecond), 0);



    Budget budget;

    EXPECT_EQ(
        budget.getTransactions().size(),
        0
    );
}



TEST_F(LifecycleEdgeCaseTest, MultipleBudgetsCanBeSwitched)
{
    const char* secondBudget[] = {
        "SECOND",
        "2026-01-01",
        "2026-12-31",
        "5000"
    };


    ASSERT_EQ(budgetAdd(4, secondBudget), 0);


    const char* switchSecond[] = {
        "SECOND"
    };


    EXPECT_EQ(cmdSwitch(1, switchSecond), 0);


    const char* switchFirst[] = {
        "TEST"
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


    EXPECT_EQ(
        budgetEdit(2, edit),
        0
    );


    const char* switchArgs[] = {
        "RENAMED"
    };


    EXPECT_EQ(
        cmdSwitch(1, switchArgs),
        0
    );
}



TEST_F(LifecycleEdgeCaseTest, RenamingBudgetPreservesTransactions)
{
    const char* txn[] = {
        "50",
        "test-category",
        "expense"
    };


    ASSERT_EQ(transactionAdd(3, txn), 0);



    const char* edit[] = {
        "name",
        "RENAMED"
    };


    ASSERT_EQ(budgetEdit(2, edit), 0);



    const char* switchArgs[] = {
        "RENAMED"
    };


    ASSERT_EQ(cmdSwitch(1, switchArgs), 0);



    Budget budget;

    EXPECT_EQ(
        budget.getTransactions().size(),
        1
    );
}



// ---------------------------------------------------------
// File corruption / recovery
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, InvalidMetadataJSONDoesNotCrash)
{
    std::ofstream file(PATH / "metadata.json");
    ASSERT_TRUE(file.is_open());
    file << "{ invalid json";
    file.close();


    EXPECT_NO_THROW(budgetList());

    std::ofstream file2(PATH / "metadata.json");
    ASSERT_TRUE(file2.is_open());
    file2 << "{}";
    file2.close();
}



TEST_F(LifecycleEdgeCaseTest, MissingCSVDoesNotCrash) {
    std::filesystem::remove(
        PATH / "TEST.csv"
    );


    EXPECT_NO_THROW(
        transactionList()
    );
}



// ---------------------------------------------------------
// Numeric boundaries
// ---------------------------------------------------------

TEST_F(LifecycleEdgeCaseTest, BudgetAcceptsLargeLimit)
{
    const char* args[] = {
        "BIG",
        "2026-01-01",
        "2026-12-31",
        "999999999999"
    };


    EXPECT_EQ(
        budgetAdd(4,args),
        0
    );
}



TEST_F(LifecycleEdgeCaseTest, TransactionSupportsDecimalValues)
{
    const char* args[] = {
        "10.99",
        "test-category",
        "expense"
    };


    EXPECT_EQ(
        transactionAdd(3,args),
        0
    );
}

