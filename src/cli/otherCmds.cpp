// TODO: Add docs for b/t/c
// TODO: Setup initBudgetManager()
// TODO: Setup cmdCurrent()
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <version.h>
#include <core/init.hpp>
#include <core/path.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int cmdHelp() {
	std::cout <<
R"(bmcli - Command line personal budgeting tool

Usage:
  bmcli [options] <command> [args]

Commands:
  init (1.0.0)							Setup Budget Manager	
  status								Condensed budget overview
  report								Full budget overview
  current (1.0.0)						Display active budget
  use | switch <budget_name> (1.0.0)	Change active budget

  b | budget [--help] (1.0.0 all sub)
    add <name> <start_date> <end_date> <limit>
	edit <field> <new_value>
	delete <budget>
	list
	
  t | transaction [--help] (1.0.0 all sub)
    add <amount> <category> [--vendor <vendor>] [--date <YYYY-MM-DD>]
    edit <id> <field> <new_value>
    delete <id>
	list

  c | category [--help]
    add <category_name> <type> <limit>
	edit <category_name> <field> <new_value>
	delete <category_name>
	list

Options:
  -h, --help (1.0.0)		Show help
  -v, --version (1.0.0)		Show version
)";
	return 0;
}

int cmdVersion() {
	std::cout << APPVERSION << "\n";
	return 0;
}

int budgetHelp() {
	std::cout << "ADD DOCS HERE.";
	return 0;
}

int transactionHelp() {
	std::cout << "ADD DOCS HERE.";
	return 0;
}

int categoryHelp() {
	std::cout << "ADD DOCS HERE.";
	return 0;
}

int cmdInit() {
	std::filesystem::path dir = setupFolder();
	std::cout << "Data saved at: " << dir << "\n";

	createFiles(dir);
	
	std::ofstream file("current");

	// TODO: Add rest of setup functions here

	std::cout << "Budget Manager setup complete.";
	return 0;
}

int cmdCurrent() {
	std::ifstream file(PATH/"current");
	std::string val;
	std::getline(file, val);
	std::cout << "Current budget: " << val << "\n";
	return 0;
}

int cmdSwitch(int argc, const char* const* argv) {
    if (argc < 1) {
        throw std::invalid_argument("Too few arguments");
    }

    std::ifstream in(PATH / "metadata.json");

    if (!in.good()) {
        throw std::runtime_error("Budget Manager has not been initialized");
    }

    json metadata;
    in >> metadata;

    std::string name = argv[0];

    if (!metadata["budgets"].contains(name)) {
        throw std::invalid_argument("Budget does not exist");
    }

    std::ofstream file(PATH / "current");

    if (!file.good()) {
        throw std::runtime_error("Could not set current budget");
    }

    file << name;

    return 0;
}
