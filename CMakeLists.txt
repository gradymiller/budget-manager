# Require cmake to be a certain version
cmake_minimum_required(VERSION 3.28)

# Get version from git
execute_process(
    COMMAND git describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Name the project, define as a C++ project
project(budget LANGUAGES CXX)

# Add the git version to the project definition
add_compile_definitions(APPVERSION="${GIT_VERSION}")

# Track files to be compiled
add_executable(budget
    src/cli/main.cpp
    src/cli/budgetCmds.cpp
    src/cli/transactionCmds.cpp
    src/cli/categoryCmds.cpp
    src/cli/otherCmds.cpp
    src/cli/utilities.cpp
)

# Add INCLUDE directories
target_include_directories(budget PRIVATE
    src/core
    ${CMAKE_BINARY_DIR}/generated
)

# Set compile features
target_compile_features(budget PRIVATE cxx_std_23)

# Set compile options
target_compile_options(budget PRIVATE
    -Wall
    -O2
)

# Create version header for --version
configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
)
