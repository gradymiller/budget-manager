#include <gtest/gtest.h>
#include <filesystem>
#include "transactionCmds.hpp"
#include "budgetCmds.hpp"

class TransactionCmdsTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        const char* argv1[] {
            "TEST",
            "2026-01-01",
            "2026-08-02",
            "1000"
        };

        budgetAdd(argv1);

        const char* argv2[] {
            "test_category"
        };

        categoryAdd(argv2);

        argv3 = new const char*[7] {
            "100.75",
            "test_category",
            "--vendor",
            "test_vendor",
            "--date",
            "2026-01-01",
            nullptr
        };
    }

    static void TearDownTestSuite() {
        delete[] argv;
        argv = nullptr;
    }

    static const char** argv;
};

const char** TransactionCmdsTest::argv = nullptr;


TEST_F(TransactionCmdsTest, AddExpenseSuccess) {
	 
}
