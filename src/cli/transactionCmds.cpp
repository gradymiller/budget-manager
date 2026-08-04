#include <iostream>
#include <string>

#include "cli/cmdTemplate.hpp"
#include "core/budget.hpp"
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

		Budget budget;
		
		budget.addTransaction(
			argv[0],
			argv[1],
			argv[2],
			date,
			vendor
		);
	});
}


int transactionEdit(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		Budget budget;

		budget.editTransaction(argv[0], argv[1], argv[2]);
	});
}


int transactionDelete(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		Budget budget;

		budget.delTransaction(argv[0]);
	});
}


int transactionList() {
	return runCommand([&]() {
		Budget budget;

		auto transactions = budget.getTransactions();

		std::cout << "ID, Amount, Category, Type, Date, Vendor\n";

		// Table-like format, everything accessed from transaction class using
		// getters or setters.
		for (const auto& [id, txn] : transactions) {
			std::cout
				<< id << ", "
				<< txn.getAmount() << ", "
				<< txn.getCategoryID() << ", "
				<< txn.getType() << ", "
				<< (txn.getDate() ? dateToStr(*txn.getDate()) : "")
				<< ", "
				<< txn.getVendor().value_or("")
				<< '\n';
		}
	});
}
