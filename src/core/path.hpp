#pragma once

#include <filesystem>
#include <cstdlib>

inline std::filesystem::path PATH =
    std::filesystem::path(std::getenv("HOME")) /
    ".local/share/budget-manager";
