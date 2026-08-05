#include <filesystem>
#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/transactionCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"
#include "core/database.hpp"
#include "core/path.hpp"

namespace fs = std::filesystem;


class TransactionCmdsTest : public ::testing::Test {

protected:

    void SetUp() override {
        fs::remove(PATH / "budget-data.db");

        ASSERT_EQ(cmdInit(), 0);

        const char* argv[] = {
            "TEST",
            "2026-01-01",
            "2026-08-02",
            "100000"
        };

        ASSERT_EQ(budgetAdd(4, argv), 0);


        const char* categoryArgs[] = {
            "test-category",
            "expense",
            "10000"
        };

        ASSERT_EQ(categoryAdd(3, categoryArgs), 0);
    }


    void TearDown() override {
        fs::remove(PATH / "budget-data.db");
    }
};


const char* const* transactionHelper() {
    static const char* argv[] = {
        "50",
        "1",
        "expense"
    };

    return argv;
}


TEST_F(TransactionCmdsTest, TransactionAddSuccess) {
    EXPECT_EQ(transactionAdd(3, transactionHelper()), 0);
}


TEST_F(TransactionCmdsTest, TransactionAddSuccessWithDate) {
    const char* argv[] = {
        "50",
        "1",
        "expense",
        "--date",
        "2026-02-01"
    };

    EXPECT_EQ(transactionAdd(5, argv), 0);
}


TEST_F(TransactionCmdsTest, TransactionAddSuccessWithVendor) {
    const char* argv[] = {
        "50",
        "1",
        "expense",
        "--vendor",
        "Walmart"
    };

    EXPECT_EQ(transactionAdd(5, argv), 0);
}


TEST_F(TransactionCmdsTest, TransactionAddRejectInvalidAmount) {
    const char* argv[] = {
        "abc",
        "1",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, argv), 1);
}


TEST_F(TransactionCmdsTest, TransactionAddRejectInvalidCategory) {
    const char* argv[] = {
        "50",
        "999",
        "expense"
    };

    EXPECT_EQ(transactionAdd(3, argv), 1);
}


TEST_F(TransactionCmdsTest, TransactionAddRejectInvalidType) {
    const char* argv[] = {
        "50",
        "1",
        "failure"
    };

    EXPECT_EQ(transactionAdd(3, argv), 1);
}


TEST_F(TransactionCmdsTest, TransactionEditSuccess) {
    ASSERT_EQ(transactionAdd(3, transactionHelper()), 0);

    const char* args[] = {
        "1",
        "amount",
        "100"
    };

    EXPECT_EQ(transactionEdit(3, args), 0);
}


TEST_F(TransactionCmdsTest, TransactionEditRejectInvalidID) {
    const char* args[] = {
        "999",
        "amount",
        "100"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}


TEST_F(TransactionCmdsTest, TransactionEditRejectInvalidField) {
    ASSERT_EQ(transactionAdd(3, transactionHelper()), 0);

    const char* args[] = {
        "1",
        "invalid",
        "100"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}


TEST_F(TransactionCmdsTest, TransactionEditRejectInvalidValue) {
    ASSERT_EQ(transactionAdd(3, transactionHelper()), 0);

    const char* args[] = {
        "1",
        "amount",
        "abc"
    };

    EXPECT_EQ(transactionEdit(3, args), 1);
}


TEST_F(TransactionCmdsTest, TransactionDeleteSuccess) {
    ASSERT_EQ(transactionAdd(3, transactionHelper()), 0);

    const char* args[] = {
        "1"
    };

    EXPECT_EQ(transactionDelete(1, args), 0);
}


TEST_F(TransactionCmdsTest, TransactionDeleteRejectInvalidTransaction) {
    const char* args[] = {
        "999"
    };

    EXPECT_EQ(transactionDelete(1, args), 1);
}


TEST_F(TransactionCmdsTest, TransactionListSuccess) {
    ASSERT_EQ(transactionAdd(3, transactionHelper()), 0);

    EXPECT_EQ(transactionList(), 0);
}
