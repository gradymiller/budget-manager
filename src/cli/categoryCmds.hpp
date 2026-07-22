#pragma once

// All functions defined here are built using a template implementing
// a try/catch block to minimize repetitive code.

// Adds a category class to the budget and sets the appropriate values.
// Vendor and Date are optional arguments.
int categoryAdd(int argc, const char* const* argv);

// Allows for editing category metadata.
// Transactions using the category cannot be edited from here.
int categoryEdit(int argc, const char* const* argv);

// Removes a category from the budget.
// Transactions must be changed to different categories before
// this will run fully.
int categoryDelete(int argc, const char* const* argv);

// lists out the categories saved in the budget and their metadata
int categoryList();
