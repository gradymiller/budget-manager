#include <iostream>


// TODO: Expand options, view real docs for examples
int printHelp() {
	std::cout <<
R"(bmcli - Command line personal budgeting tool

Usage:
  bmcli [options] <command> [args]

Commands:
  init							Setup Budget Manager	
  status						Condensed budget overview
  report						Full budget overview
  current						Display active budget
  use | switch <budget_name>	Change active budget

  b | budget [--help]
    add <name> <start_date> <end_date> <limit>
	edit <field> <new_value>
	delete <budget>
	list
	
  t | transaction [--help]
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
  -h, --help				Show help
  -v, --version				Show version
)";
	return 0;
}

// TODO: have version auto update from something on github?
int printVersion() {
	std::cout << "budget version 1.0.0\n";
		return 0;
}
