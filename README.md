# FinanceTracker

A desktop app for tracking personal income, expenses, and budgets. Built with C++17 and Qt6.

## Features

- Log income and expense transactions with title, amount, category, and date
- View running balance, total income, and total expenses at a glance
- Dashboard with a spending-by-category pie chart and a monthly income/expense bar chart
- Export all transactions to CSV
- Data stored locally in a SQLite database (`~/finance_tracker.sqlite`)

## Install

Download the latest `.deb` from the [Releases](https://github.com/NazmulHossain9/FinanceTracker/releases) page and install:

```bash
sudo dpkg -i FinanceTracker-1.0-amd64.deb
```

Requires Qt6 runtime libraries (`libqt6core6`, `libqt6widgets6`, `libqt6sql6`, `libqt6charts6`).

## Build from source

**Prerequisites:** CMake 3.16+, Qt6 (Core, Widgets, Sql, Charts), a C++17 compiler.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/FinanceTracker
```

## Usage

| Action | How |
|---|---|
| Add a transaction | Click **Add Transaction** |
| Delete a transaction | Select a row → click **Delete Selected** |
| View charts | **View → Dashboard** |
| Export to CSV | **File → Export CSV…** |
