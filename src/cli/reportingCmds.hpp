#pragma once

#include <string>

// Outputs a simple json, outlining the categories for the current budget and
// their usages.
int cmdStatus();

// Provides a more in depth report of the current state of the budget and
// outlines some statistical measures.
int cmdReport();

// Helper for generating progress bars using percentages
std::string progressBar(double percent, int width);
