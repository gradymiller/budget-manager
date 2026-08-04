#include <iostream>
#include <string>
#include <vector>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
#include "core/category.hpp"
#include "core/database.hpp"

int categoryAdd(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Database db("budget-data.db");
		Budget budget = db.loadBudget();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		auto category = budget.addCategory(argv[0], argv[1], argv[2]);
		auto category_id = db.createCategory(category);
		std::cout << "Category ID: " << category_id << '\n';
	});
}


int categoryEdit(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Database db("budget-data.db");
		Budget budget = db.loadBudget();

		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		auto category_id = budget.editCategory(argv[0], argv[1], argv[2]);
		db.updateCategory(budget.getCategory(category_id));
	});
}


int categoryDelete(int argc, const char* const argv[]) {
	return runCommand([&]() {
		Database db("budget-data.db");
		Budget budget = db.loadBudget();

		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		auto category_id = budget.delCategory(argv[0]);
		db.deleteCategory(category_id);
	});
}


int categoryList() {
	return runCommand([&]() {
			std::cout << "NOTHING HERE RIGHT NOW" << '\n';
	});
}
