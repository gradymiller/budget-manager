#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"

class CategoryCmdsTest : public ::testing::Test {
protected:
    void SetUp() override {
		try {
			cmdInit();
			const char* cleanup[] = {"TEST"};
			budgetDelete(1, cleanup);

		} catch (...) {}
		
        const char* argv1[] = {
            "TEST",
            "2026-01-01",
            "2026-08-02",
            "1000"
        };

        budgetAdd(4, argv1);

        const char* argv2[] = {"TEST"};
        cmdSwitch(1, argv2);
    }

    void TearDown() override {
        const char* argv[] = {"TEST"};
        budgetDelete(1, argv);
    }
};

const char* const* categoryHelper() {
    static const char* argv[] = {
        "test-category",
        "expense",
        "100"
    };

    return argv;
}

TEST_F(CategoryCmdsTest, categoryAddSuccess) {
    const char* argv[] = {
        "add-success-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidName) {
    const char* argv[] = {
        "-invalid-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidType) {
    const char* argv[] = {
        "invalid-type-category",
        "idk",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidLimit) {
    const char* argv[] = {
        "invalid-limit-category",
        "expense",
        "100a"
    };

    EXPECT_EQ(categoryAdd(3, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryEditSuccess) {
    const char* argv[] = {
        "edit-success-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);

    const char* args[] = {
        "edit-success-category",
        "name",
        "edited-category"
    };

    EXPECT_EQ(categoryEdit(3, args), 0);
}

TEST_F(CategoryCmdsTest, categoryEditRejectInvalidCategoryName) {
    const char* argv[] = {
        "invalid-edit-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);

    const char* args[] = {
        "invalid-edit-category",
        "name",
        "@@@a"
    };

    EXPECT_EQ(categoryEdit(3, args), 1);
}

TEST_F(CategoryCmdsTest, categoryEditRejectInvalidCategoryType) {
    const char* argv[] = {
        "invalid-type-edit-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);

    const char* args[] = {
        "invalid-type-edit-category",
        "type",
        "failure"
    };

    EXPECT_EQ(categoryEdit(3, args), 1);
}

TEST_F(CategoryCmdsTest, categoryDeleteSuccess) {
    const char* argv[] = {
        "delete-success-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);

    const char* args[] = {
        "delete-success-category"
    };

    EXPECT_EQ(categoryDelete(1, args), 0);
}

TEST_F(CategoryCmdsTest, categoryDeleteRejectInvalidCategory) {
    const char* argv[] = {
        "delete-invalid-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);

    const char* args[] = {
        "category-does-not-exist"
    };

    EXPECT_EQ(categoryDelete(1, args), 1);
}

TEST_F(CategoryCmdsTest, categoryListSuccess) {
    const char* argv[] = {
        "list-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);

    EXPECT_EQ(categoryList(), 0);
}
