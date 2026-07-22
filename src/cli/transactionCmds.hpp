#pragma once

// All functions defined here are built using a template implementing
// a try/catch block to minimize repetitive code.

// Adds a transaction to the budget's csv file.
// This is the only function that updates the transaction ID
// for use in future transactions.
int transactionAdd(int argc, const char* const argv[]);

// Allows for editing a transaction's data.
// Overwrites the csv file with the updated data, does not just modify
// the single entry.
int transactionEdit(int argc, const char* const argv[]);

// Removes a transaction from the budget's transactions.
// There will be a gap in the transaction IDs as not transaction
// will ever fill the spot of the deleted transaction. The IDs will
// continue incrementing upwards.
int transactionDelete(int argc, const char* const argv[]);

// Lists out the transactions in a table-like format to stdout.
int transactionList();
