#pragma once

int cmdHelp();
int cmdVersion();

int budgetHelp();
int transactionHelp();
int categoryHelp();

int cmdInit();
int cmdCurrent();
int cmdSwitch(int argc, char** argv);
