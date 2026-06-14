// TODO: Add docs for b/t/c
// TODO: Setup initBudgetManager()
// TODO: Setup cmdSwitch()
#include <iostream>
#include <version.h>

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
    add <category>
	edit | rename <category> <new_value>
	delete <category>
	list

Options:
  -h, --help (1.0.0)		Show help
  -v, --version (1.0.0)		Show version
)";
	return 0;
}

int cmdVersion() {
	std::cout << APP_VERSION << "\n";
	return 0;
}

int budgetHelp() {
	std::cout << "ADD DOCS HERE.";
}

int transactionHelp() {
	std::cout << "ADD DOCS HERE.";
}

int categoryHelp() {
	std::cout << "ADD DOCS HERE.";
}

int cmdInit() {
	try {
		//initBudgetManager();
		std::cout << "Budget Manager setup complete.";
		return 0;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
}

int cmdSwitch(char** args) {
	// Make this
}
