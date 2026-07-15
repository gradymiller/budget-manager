// TODO: Make load() and these should all pass
#include <gtest/gtest.h>

#include "cli/budgetCmds.hpp"
#include "cli/categoryCmds.hpp"
#include "cli/otherCmds.hpp"

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

		const char* argv2[] = {"TEST"};
		cmdSwitch(1, argv2);
    }

    static void TearDownTestSuite() {
		const char* argv3[] = {"TEST"};
		budgetDelete(argv3);
    }
};

const char* const* categoryHelper() {
	static const char* argv[] = {"test-category", "expense", "100"};
	return argv;
}

TEST_F(CategoryCmdsTest, categoryAddSuccess) {
    const char* const* argv = categoryHelper();

    EXPECT_EQ(categoryAdd(3, argv), 0);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidName) {
	const char* argv[] = {"-1d@s", "expense", "100"};

    EXPECT_EQ(categoryAdd(3, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidType) {
	const char* argv[] = {"test_category", "idk", "100"};

    EXPECT_EQ(categoryAdd(3, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryAddRejectInvalidLimit) {
	const char* argv[] = {"test_category", "income", "100a"};

    EXPECT_EQ(categoryAdd(3, argv), 1);
}

TEST_F(CategoryCmdsTest, categoryEditSuccess) {
    const char* const* argv = categoryHelper();
	categoryAdd(3, argv);

	const char* args[] = {"test-category", "name", "groceries"};
    EXPECT_EQ(categoryEdit(3, args), 0);
}

TEST_F(CategoryCmdsTest, categoryEditRejectInvalidCategoryName) {
    const char* const* argv = categoryHelper();
	categoryAdd(3, argv);

	const char* args[] = {"invalidCategory", "name", "@@@a"};
    EXPECT_EQ(categoryEdit(3, args), 1);
}

TEST_F(CategoryCmdsTest, categoryEditRejectInvalidCategoryType) {
    const char* const* argv = categoryHelper();
	categoryAdd(3, argv);

	const char* args[] = {"test-category", "type", "failure"};
    EXPECT_EQ(categoryEdit(3, args), 1);
}

TEST_F(CategoryCmdsTest, categoryDeleteSuccess) {
    const char* const* argv = categoryHelper();
	categoryAdd(3, argv);

	const char* args[] = {"test-category"};
    EXPECT_EQ(categoryDelete(1, args), 0);
}

TEST_F(CategoryCmdsTest, categoryDeleteRejectInvalidCategory) {
    const char* const* argv = categoryHelper();
	categoryAdd(3, argv);

	const char* args[] = {"invalid-category"};
    EXPECT_EQ(categoryDelete(1, args), 1);
}

TEST_F(CategoryCmdsTest, categoryListSuccess) {
    const char* const* argv = categoryHelper();
	categoryAdd(3, argv);

    EXPECT_EQ(categoryList(), 0);
}
