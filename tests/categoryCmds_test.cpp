#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"

class CategoryCmdsTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Create a test budget
        const char* argv1[] = {
            "TEST",
            "2026-01-01",
            "2026-08-02",
            "1000"
        };

        budgetAdd(argv1);
    }

    static void TearDownTestSuite() {
		budgetDelete({"TEST"});
    }
};

const char* const* successHelper() {
	static const char* argv[] = {"test-category"};
	return argv;
}

TEST_F(CategoryCmdsTest, categoryAddSuccess) {
    const char* const* argv = successHelper();

    EXPECT_EQ(categoryAdd(1, argv), 0);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidName1) {
	const char* argv[] = {"-1ds"};

    EXPECT_EQ(categoryAdd(1, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidName2) {
	const char* argv[] = {"asdf@asdf"};

    EXPECT_EQ(categoryAdd(1, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryEditSuccess) {
    const char* const* argv = successHelper();
	categoryAdd(1, argv);

	const char* args[] = {"test-category", "groceries"};
    EXPECT_EQ(categoryEdit(2, argv), 0);
}

TEST_F(CategoryCmdsTest, categoryEditRejectInvalidOldCategory) {
    const char* const* argv = successHelper();
	categoryAdd(1, argv);

	const char* args[] = {"invalidCategory", "groceries"};
    EXPECT_EQ(categoryEdit(2, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryEditRejectInvalidNewCategory) {
    const char* const* argv = successHelper();
	categoryAdd(1, argv);

	const char* args[] = {"test-category", "category!1-"};
    EXPECT_EQ(categoryEdit(2, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryDeleteSuccess) {
    const char* const* argv = successHelper();
	categoryAdd(1, argv);

	const char* args[] = {"test-category"};
    EXPECT_EQ(categoryDelete(1, argv), 0);
}

TEST_F(CategoryCmdsTest, categoryDeleteRejectInvalidCategory) {
    const char* const* argv = successHelper();
	categoryAdd(1, argv);

	const char* args[] = {"invalid-category"};
    EXPECT_EQ(categoryDelete(1, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryListSuccess) {
    const char* const* argv = successHelper();
	categoryAdd(1, argv);

    EXPECT_EQ(categoryList(), 0);
}
