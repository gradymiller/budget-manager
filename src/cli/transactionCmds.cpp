#include <iostream>
#include <stdexcept>
#include "core/budget.hpp"
#include "core/transaction.hpp"


int transactionAdd(int argc, const char* const*) {
	/*try {
		if (argc < 2) {
			throw std::invalid_argument("Too few arguments entered");
		}

		Budget budget;
		budget.load();

		Transaction txn;

		txn.setAmount(argv[0]);
		txn.setCategory(argv[1]);

		for (int i = 2; i < argc; ++i) {
			std::string arg = argv[i];

			if (arg == "--vendor") {
				if (i + 1 > argc) {
					throw std::invalid_argument("The --vendor flag requires an argument after it");
				}
				txn.setVendor(argv[++i]);

			} else if (arg == "--date") {
				if (i + 1 > argc) {
					throw std::invalid_argument("The --date flag requires an argument after it");
				}
				txn.setDate(argv[++i]);
			}
		}

		budget.addTransaction(txn);
		budget.saveTransactions();

	} catch (const std::invalid_argument& e) {
		std::cerr << "Invalid Argument: " << e.what() << "\n";
		return 1;
	}*/
	return 0;
}

int transactionEdit(int argc, const char* const*) {
	if (argc < 3) {
		throw std::invalid_argument("Too few arguments");
	}


	return 0;
}

int transactionDelete(int argc, const char* const*) {
	return 0;
}

int transactionList() {
	return 0;
}
