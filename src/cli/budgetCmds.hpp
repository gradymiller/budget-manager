#pragma once

// All functions defined here are built using a template implementing
// a try/catch block to minimize repetitive code.

// Creates a new budget, saves the metadata and creates
// a csv file to store transactions in.
int budgetAdd(int argc, const char* const* argv);

// Allows for editing of budget metadata.
// This could be changing the name, the limit, dates, etc.
int budgetEdit(int argc, const char* const* argv);

// Deletes a budget, removes it from the metadata file
// and removes the csv file associated with the budget.
int budgetDelete(int argc, const char* const* argv);

// Sends a list of the budgets and their metadata to stdout for viewing.
int budgetList();
