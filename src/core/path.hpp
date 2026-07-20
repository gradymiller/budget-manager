#pragma once

#include <cstdlib>
#include <filesystem>

// This is the main path used throughout the program
// TODO: Make this editable from a config file
inline std::filesystem::path PATH =
    std::filesystem::path(std::getenv("HOME")) /
    ".local/share/budget-manager";
