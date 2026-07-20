#pragma once

#include <filesystem>

// Make dedicated budget-manager folder to be used be the tool.
std::filesystem::path setupFolder();

// Create data files that will be written to when a budget is added.
void createFiles(std::filesystem::path dir);
