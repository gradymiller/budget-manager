#pragma once

#include <iostream>
#include <stdexcept>

template <typename Func>
int runCommand(Func&& func) {
    try {
        func();
        return 0;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid Argument: " << e.what() << '\n';
        return 1;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
