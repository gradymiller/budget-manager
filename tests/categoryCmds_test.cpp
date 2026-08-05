#include <filesystem>
#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"
#include "core/database.hpp"
#include "core/path.hpp"

namespace fs = std::filesystem;


class CategoryCmdsTest : public ::testing::Test {

protected:

    void SetUp() override {
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
        fs::remove(PATH / "budget-data.db");
    }
};


TEST_F(CategoryCmdsTest, CategoryAddSuccess) {
    const char* argv[] = {
        "add-success-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 0);
}


TEST_F(CategoryCmdsTest, CategoryAddRejectInvalidName) {
    const char* argv[] = {
        "-invalid-category",
        "expense",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 1);
}


TEST_F(CategoryCmdsTest, CategoryAddRejectInvalidType) {
    const char* argv[] = {
        "invalid-type-category",
        "idk",
        "100"
    };

    EXPECT_EQ(categoryAdd(3, argv), 1);
}


TEST_F(CategoryCmdsTest, CategoryAddRejectInvalidLimit) {
    const char* argv[] = {
        "invalid-limit-category",
        "expense",
        "100a"
    };

    EXPECT_EQ(categoryAdd(3, argv), 1);
}


TEST_F(CategoryCmdsTest, CategoryEditSuccess) {
    const char* argv[] = {
        "edit-success-category",
        "expense",
        "100"
    };

    ASSERT_EQ(categoryAdd(3, argv), 0);


    const char* args[] = {
        "1",
        "name",
        "edited-category"
    };

    EXPECT_EQ(categoryEdit(3, args), 0);
}


TEST_F(CategoryCmdsTest, CategoryEditRejectInvalidCategoryName) {
    const char* argv[] = {
        "invalid-edit-category",
        "expense",
        "100"
    };

    ASSERT_EQ(categoryAdd(3, argv), 0);


    const char* args[] = {
        "1",
        "name",
        "@@@a"
    };

    EXPECT_EQ(categoryEdit(3, args), 1);
}


TEST_F(CategoryCmdsTest, CategoryEditRejectInvalidCategoryType) {
    const char* argv[] = {
        "invalid-type-edit-category",
        "expense",
        "100"
    };

    ASSERT_EQ(categoryAdd(3, argv), 0);


    const char* args[] = {
        "1",
        "type",
        "failure"
    };

    EXPECT_EQ(categoryEdit(3, args), 1);
}


TEST_F(CategoryCmdsTest, CategoryDeleteSuccess) {
    const char* argv[] = {
        "delete-success-category",
        "expense",
        "100"
    };

    ASSERT_EQ(categoryAdd(3, argv), 0);


    const char* args[] = {
        "1"
    };

    EXPECT_EQ(categoryDelete(1, args), 0);
}


TEST_F(CategoryCmdsTest, CategoryDeleteRejectInvalidCategory) {
    const char* argv[] = {
        "delete-invalid-category",
        "expense",
        "100"
    };

    ASSERT_EQ(categoryAdd(3, argv), 0);


    const char* args[] = {
        "999"
    };

    EXPECT_EQ(categoryDelete(1, args), 1);
}


TEST_F(CategoryCmdsTest, CategoryListSuccess) {
    const char* argv[] = {
        "list-category",
        "expense",
        "100"
    };

    ASSERT_EQ(categoryAdd(3, argv), 0);

    EXPECT_EQ(categoryList(), 0);
}
