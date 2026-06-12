#include <iostream>
#include <version.h>


// TODO: Expand options, view real docs for examples
int printHelp() {
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

// TODO: have version auto update from something on github?
int printVersion() {
	std::cout << APP_VERSION << "\n";
		return 0;
}
