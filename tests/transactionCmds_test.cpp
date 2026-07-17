#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/transactionCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"

class TransactionCmdsTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Create test budget
        const char* argv1[] = {
            "TEST",
            "2026-01-01",
            "2026-08-02",
            "1000"
        };

        budgetAdd(argv1);

        const char* argv2[] = {"TEST"};
        cmdSwitch(1, argv2);

        // Add category needed for transactions
        const char* categoryArgs[] = {
            "test-category",
            "expense",
            "100"
        };

        categoryAdd(3, categoryArgs);
    }

    static void TearDownTestSuite() {
        const char* argv3[] = {"TEST"};
        budgetDelete(argv3);
    }
};

const char* const* transactionHelper() {
    static const char* argv[] = {
        "50",
        "test-category",
        "expense"
    };

    return argv;
}

TEST_F(TransactionCmdsTest, transactionAddSuccess) {
    const char* const* argv = transactionHelper();

    EXPECT_EQ(transactionAdd(3, argv), 0);
}

TEST_F(TransactionCmdsTest, transactionAddSuccessWithDate) {
    const char* argv[] = {
        "50",
        "test-category",
        "expense",
        "--date",
        "2026-02-01"
    };

    EXPECT_EQ(transactionAdd(5, argv), 0);
}

TEST_F(TransactionCmdsTest, transactionAddSuccessWithVendor) {
    const char* argv[] = {
        "50",
        "test-category",
        "expense",
        "--vendor",
        "Walmart"
    };

    EXPECT_EQ(transactionAdd(5, argv), 0);
}

TEST_F(TransactionCmdsTest, transactionAddRejectInvalidAmount) {
    const char* argv[] = {
        "abc",
        "test-category",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, argv), 1);
}

TEST_F(TransactionCmdsTest, transactionAddRejectInvalidCategory) {
    const char* argv[] = {
        "50",
        "invalid-category",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, argv), 1);
}

TEST_F(TransactionCmdsTest, transactionAddRejectInvalidType) {
    const char* argv[] = {
        "50",
        "test-category",
        "failure"
    };

    EXPECT_EQ(transactionAdd(3, argv), 1);
}

TEST_F(TransactionCmdsTest, transactionEditSuccess) {
    const char* const* argv = transactionHelper();

    transactionAdd(3, argv);

    const char* args[] = {
        "1",
        "amount",
        "100"
    };

    EXPECT_EQ(transactionEdit(3, args), 0);
}

TEST_F(TransactionCmdsTest, transactionEditRejectInvalidID) {
    const char* args[] = {
        "999",
        "amount",
        "100"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}

TEST_F(TransactionCmdsTest, transactionEditRejectInvalidField) {
    const char* const* argv = transactionHelper();

    transactionAdd(3, argv);

    const char* args[] = {
        "1",
        "invalid",
        "100"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}

TEST_F(TransactionCmdsTest, transactionEditRejectInvalidValue) {
    const char* const* argv = transactionHelper();

    transactionAdd(3, argv);

    const char* args[] = {
        "1",
        "amount",
        "abc"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}

TEST_F(TransactionCmdsTest, transactionDeleteSuccess) {
    const char* const* argv = transactionHelper();

    transactionAdd(3, argv);

    const char* args[] = {"1"};

    EXPECT_EQ(transactionDelete(1, args), 0);
}

TEST_F(TransactionCmdsTest, transactionDeleteRejectInvalidTransaction) {
    const char* args[] = {"999"};

    EXPECT_EQ(transactionDelete(1, args), 1);
}

TEST_F(TransactionCmdsTest, transactionListSuccess) {
    const char* const* argv = transactionHelper();

    transactionAdd(3, argv);

    EXPECT_EQ(transactionList(), 0);
}
