#pragma once

#include <cstdlib>
#include <filesystem>

const std::filesystem::path PATH = [] {
    const char* test_path = std::getenv("BMCLI_TEST_PATH");

    if (test_path != nullptr) {
        return std::filesystem::path(test_path);
    }

    return std::filesystem::path(std::getenv("HOME")) /
           ".local/share/budget-manager";
}();
