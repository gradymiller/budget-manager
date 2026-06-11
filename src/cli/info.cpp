#include <iostream>


// TODO: Expand options, view real docs for examples
int printHelp() {
	std::cout <<
R"(budget - Personal budgeting tool

Usage:
  budget [global options] <command> [args]

Commands:
  init
  create [-n <name> | --name <name>] <start_date> <end_date>
  add <amount> <category> <vendor> <date>
  edit <id> <field> <new_value>
  delete <id>
  show 

Options:
  -h, --help		Show help
  -v, --version		Show version
  -b, --budget		Specify budget
)";
	return 0;
}

// TODO: have version auto update from something on github?
int printVersion() {
	std::cout << "budget version 1.0.0\n";
		return 0;
}
