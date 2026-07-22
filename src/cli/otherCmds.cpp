// TODO: finish setting um cmdInit()

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <version.h>

#include <nlohmann/json.hpp>

#include <core/init.hpp>
#include <core/path.hpp>

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
    add <name> <start_date> <end_date> <limit>
    edit <field> <new_value>
    delete <budget>
    list
	
  t[ransaction]
    add <amount> <category> <type> [--date <YYYY-MM-DD>] [--vendor <vendor>]
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
	std::filesystem::path dir = setupFolder();
	std::cout << "Data saved at: " << dir << "\n";

	// Utility function
	createFiles(dir);
	
	std::ofstream file(PATH / "current");

	// TODO: Add rest of setup functions here

	std::cout << "Budget Manager setup complete." << '\n';
	return 0;
}

int cmdCurrent() {
	std::ifstream file(PATH / "current");
	std::string val;
	std::getline(file, val);
	std::cout << "Current budget: " << val << '\n';
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

	// Metadata file is source of truth
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
