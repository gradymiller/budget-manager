#include <iostream>
#include <string>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
#include "core/category.hpp"
#include "core/database.hpp"
#include "core/path.hpp"

int categoryAdd(int argc, const char* const argv[]) {
    return runCommand([&]() {
        if (argc < 3) {
            throw std::invalid_argument(
                "Too few arguments");
        }

        Database db(PATH / "budget-data.db");
        Budget budget = db.loadBudget();

        Category category = budget.addCategory(argv[0], argv[1], argv[2]);

        // Associate the category with the current budget.
        category.setBudgetID(budget.getID());

        // Store in the database.
        int category_id = db.createCategory(category);

        // Keep the in-memory object consistent.
        category.setID(category_id);

        std::cout << "Category ID: " << category_id << '\n';
    });
}

int categoryEdit(int argc, const char* const argv[]) {
    return runCommand([&]() {
        if (argc < 3) {
            throw std::invalid_argument(
                "Too few arguments");
        }

        Database db(PATH / "budget-data.db");
        Budget budget = db.loadBudget();

        int category_id = budget.editCategory(argv[0], argv[1], argv[2]);

        db.updateCategory(budget.getCategory(category_id));
    });
}

int categoryDelete(int argc, const char* const argv[]) {
    return runCommand([&]() {
        if (argc < 1) {
            throw std::invalid_argument(
                "Too few arguments");
        }

        Database db(PATH / "budget-data.db");
        Budget budget = db.loadBudget();

        int category_id = budget.delCategory(argv[0]);

        db.deleteCategory(category_id);
    });
}

int categoryList() {
    return runCommand([&]() {
		Database db(PATH / "budget-data.db");
		std::vector<Category> categories = db.readCategories();

		std::cout << "ID, Budget ID, Name, Type, Limit, Usage\n";
		for (auto category : categories) {
			std::cout << category.getID() << ", "
					  << category.getBudgetID() << ", "
					  << category.getName() << ", "
					  << category.getType() << ", "
					  << category.getLimit() << ", "
					  << category.getUsage() << '\n';
		}
    });
}
