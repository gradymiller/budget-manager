# Budget Manager

This is a terminal-based budget application written in C++.

## Setup:

```
make
```

```
make install
```
1. Uses ncurses library, compile with:
- g++ -Isrc \
src/core/*.cpp \
src/cli/*.cpp \
-o budget-cli

- g++ -Isrc \
src/core/*.cpp \
src/tui/*.cpp \
-lncurses \
-o budget-tui

- Use "-Isrc" because there are 2 main.cpp files to make the cli and tui
  executable using the same core functionality


### Features:

1. Adding budgets
- Create as many budgets as you want
- They can be independent or link together

2. Modify budgets
- Change monthly expense amounts
- Add custom categories to keep track of where the money goes

3. View analytics
- See spending stats based on the month, year, or category
- Predict spending amounts based on spending history

4. External Data Importing
- Track receipts and payments through email

5. Data Management
- Local database
- Create multiple databases
- Look at spending history
- ALT: Use a json or txt file that is stored in shadow

6. Security
- Encrypted, locally hosted storage
- Open source, manually compiled
