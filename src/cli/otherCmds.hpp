#pragma once

// Prints a simple outline with info on using the tool.
int cmdHelp();

// Outputs the current version of the executable based
// on git tags and commits.
int cmdVersion();

// Sets up the basic directory and files needed to run
// the budget-manager tool.
int cmdInit();

// Returns the current active budget which is stored in a text file.
int cmdCurrent();

// Switches the active budget by saving a new name in the text file.
int cmdSwitch(int argc, const char* const* argv);
