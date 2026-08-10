#include <iostream>
#include <string>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
#include "core/database.hpp"
#include "core/path.hpp"
#include "core/transaction.hpp"
#include "core/utils.hpp"

int transactionAdd(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments entered");
		}

		std::string date = "";
		std::string vendor = "";

		for (int i = 3; i < argc; ++i) {
			std::string arg = argv[i];

			// parses optional date arg that requires a flag to use
			if (arg == "--date") {
				if (i + 1 >= argc) {
					throw std::invalid_argument("The --date flag requires an argument after it");
				}

				date = argv[++i];

			// parses optional vendor arg that requires a flag to use
			} else if (arg == "--vendor") {
				if (i + 1 >= argc) {
					throw std::invalid_argument("The --vendor flag requires an argument after it");
				}

				vendor = argv[++i];
			}
		}

		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();
		
		auto txn = budget.addTransaction(
			argv[0],
			argv[1],
			argv[2],
			date,
			vendor
		);

		auto txn_id = db.createTransaction(txn);
		db.updateCategory(budget.getCategory(txn.getCategoryID()));

		std::cout << "Transaction ID: " << txn_id << '\n';
	});
}


int transactionEdit(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();

		auto txn_id = budget.editTransaction(argv[0], argv[1], argv[2]);

		Transaction txn = budget.getTransaction(txn_id);
		db.updateTransaction(txn);

		db.updateCategory(budget.getCategory(txn.getCategoryID()));
	});
}


int transactionDelete(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();

		Transaction txn = budget.getTransaction(std::stoi(argv[0]));
		int category_id = txn.getCategoryID();

		budget.delTransaction(argv[0]);
		db.deleteTransaction(txn.getID());
		db.updateCategory(budget.getCategory(category_id));
	});
}

int transactionList() {
    return runCommand([&]() {
        Database db(PATH / "budget-data.db");
        db.readTransactions();
    });
}
