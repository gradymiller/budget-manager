#include <iostream>
#include <string>

#include "core/budget.hpp"
#include "core/transaction.hpp"
#include "core/utils.hpp"
#include "cli/cmdTemplate.hpp"


int transactionAdd(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments entered");
		}

		std::string date = "";
		std::string vendor = "";

		for (int i = 3; i < argc; ++i) {
			std::string arg = argv[i];

			if (arg == "--date") {
				if (i + 1 >= argc) {
					throw std::invalid_argument("The --date flag requires an argument after it");
				}

				date = argv[++i];

			} else if (arg == "--vendor") {
				if (i + 1 >= argc) {
					throw std::invalid_argument("The --vendor flag requires an argument after it");
				}

				vendor = argv[++i];
			}
		}

		Budget budget;
		budget.load();
		
		std::cout << "date=[" << date << "]\n";
		std::cout << "vendor=[" << vendor << "]\n";

		budget.addTransaction(
			argv[0],
			argv[1],
			argv[2],
			date,
			vendor
		);

		budget.saveTransactions();
	});
}


int transactionEdit(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 3) {
			throw std::invalid_argument("Too few arguments");
		}

		Budget budget;
		budget.load();

		budget.editTransaction(argv[0], argv[1], argv[2]);

		budget.saveTransactions();
	});
}


int transactionDelete(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		Budget budget;
		budget.load();

		budget.delTransaction(argv[0]);

		budget.saveTransactions();
	});
}


int transactionList() {
	return runCommand([&]() {
		Budget budget;
		budget.load();

		auto transactions = budget.getTransactions();

		std::cout << "ID, Amount, Category, Type, Date, Vendor\n";

		for (const auto& [id, txn] : transactions) {
			std::cout
				<< id << ", "
				<< txn.getAmount() << ", "
				<< txn.getCategory() << ", "
				<< txn.getType() << ", "
				<< (txn.getDate() ? formatDate(*txn.getDate()) : "")
				<< ", "
				<< txn.getVendor().value_or("")
				<< '\n';
		}
	});
}
