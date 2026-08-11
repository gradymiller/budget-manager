#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <version.h>

#include <nlohmann/json.hpp>

#include "cli/cmdTemplate.hpp"
#include "core/database.hpp"
#include "core/init.hpp"
#include "core/path.hpp"

using json = nlohmann::json;

int cmdHelp() {
	std::cout <<
R"(
bmcli - Command line personal budgeting tool

Usage:
  bmcli [options] <command> [args]

Options:
  -h, --help		Show help
  -v, --version		Show version

Commands:
  init                          Setup Budget Manager	
  status                        Condensed budget overview
  report                        Full budget overview
  current                       Display active budget
  use | switch <budget_name>    Change active budget

  b[udget]
    add <name> <start_date> <end_date>
    edit <field> <new_value>
    delete <budget>
    list
	
  t[ransaction]
    add <amount> <category_id> <type> [--date <YYYY-MM-DD>] [--vendor <vendor>]
    edit <id> <field> <new_value>
    delete <id>
    list

  c[ategory]
    add <category_name> <type> <limit>
    edit <category_name> <field> <new_value>
    delete <category_name>
    list

)";
	return 0;
}

int cmdVersion() {
	std::cout << "bmcli - budget manager command line tool\nVersion: " << APPVERSION << '\n';
	return 0;
}

int cmdInit() {
	return runCommand([&]() {
		setupFolder();
		std::cout << "Data saved at: " << PATH << "\n";

		// Inits database
		createFiles();

		// TODO: Add rest of setup functions here
	});
}

int cmdCurrent() {
	return runCommand([&]() {
		Database db(PATH / "budget-data.db");

		auto current = db.getSetting("current_budget");

		if (current.has_value()){
			std::cout << current.value() << '\n';

		} else {
			throw std::runtime_error("No budget is selected");
		}
	});
}

int cmdSwitch(int argc, const char* const* argv) {
	return runCommand([&]() {
		if (argc < 1) {
			throw std::invalid_argument("Too few arguments");
		}

		Database db(PATH / "budget-data.db");
		std::vector<Budget> budgets = db.readBudgets();

		int curr_id = std::stoi(argv[0]);

		bool found = false;
		for (auto budget : budgets) {
			if (curr_id == budget.getID()) {
				found = true;
				break;
			}
		}

		if (!found) {
			throw std::runtime_error("Invalid budget ID, cannot switch");
		}

		db.setSetting("current_budget", argv[0]);
	});
}
