#include <iostream>
#include <stdexcept>
#include "core/budget.hpp"
#include "core/transaction.hpp"


int transactionAdd(int argc, const char* const* argv) {
	try {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments entered");
		}

		for (int i = 3; i < argc; ++i) {
			std::string arg = argv[i];

			if (arg == "--date") {
				if (i + 1 > argc) {
					throw std::invalid_argument("The --date flag requires an argument after it");
				}
				vendor = argv[++i];

			} else if (arg == "--vendor") {
				if (i + 1 > argc) {
					throw std::invalid_argument("The --vendor flag requires an argument after it");
				}
				std::string date = argv[++i];
			}
		}

		Budget budget;
		budget.load();

		budget.addTransaction(argv[0], argv[1], argv[2], date, vendor);
		budget.saveTransactions();
		return 0;

	} catch (const std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}

int transactionEdit(int argc, const char* const* argv) {
	try {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		Budget budget;
		budget.load();

		budget.editTransaction(argv[0], argv[1], argv[2]);
		budget.saveTransactions();
		return 0;

	} catch (std::invalid_argument& e) {
		throw std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}

int transactionDelete(int argc, const char* const* argv) {
	try {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		Budget.budget;
		budget.load();

		budget.delTransaction(argv[0]);
		return 0;

	} catch (const std::invalid_argument& e) {
		throw std::cerr << "Invalid Argument: " << e.what() << '\n';
		return 1;
	}
}

int transactionList() {
	Budget budget;
	budget.load();

	auto transactions = budget.getTransactions();
	
	std::cout << "ID, Amount, Category, Type, Date, Vendor\n";

	for (Transaction txn : transactions) {
		std::cout << txn.getID() << ", " << txn.getAmount() << ", " << txn.getCategory() << ", " << txn.getType() << ", " << txn.getDate() << ", " << txn.getVendor() << '\n';
	}

	return 0;
}
