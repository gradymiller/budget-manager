# Budget Manager

This is a terminal-based budget application written in C++.

## Setup:

First, clone the repository to your local machine.

Run the bash script to set up and compile the budget manager tool to your
system.
```
cd budget-manager
./setup.sh
```

Once the compilation is finished there will be 2 executables located in
"./build".

The bmcli executable is to run the tool and the bmcli_test runs the unit tests.
The main executable can be run as is or can be added to your path for ease of
use.

#### Note:
The tool will leave it's files in ~/.local/share/budget-manager

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
