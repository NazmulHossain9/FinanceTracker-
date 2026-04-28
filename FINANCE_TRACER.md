# FinanceTracker

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                        main.cpp                         │
│  Initializes QApplication, DatabaseManager, MainWindow  │
└───────────────────────┬─────────────────────────────────┘
                        │
          ┌─────────────▼──────────────┐
          │         UI Layer           │
          │                            │
          │  MainWindow (QMainWindow)  │◄──── AddTransactionDialog
          │  - Transaction table       │      (QDialog)
          │  - Balance summary bar     │
          │  - Menu bar                │
          │            │               │
          │   DashboardWidget          │
          │   (QWidget / popup)        │
          │   - Pie chart (by cat.)    │
          │   - Bar chart (by month)   │
          └─────────────┬──────────────┘
                        │ reads/writes via
          ┌─────────────▼──────────────┐
          │      DatabaseManager       │
          │      (Singleton)           │
          │  SQLite via Qt6::Sql       │
          │  ~/finance_tracker.sqlite  │
          └─────────────┬──────────────┘
                        │ returns / accepts
          ┌─────────────▼──────────────┐
          │        Models              │
          │                            │
          │  Transaction               │
          │  Account                   │
          │  Budget                    │
          └────────────────────────────┘

  Utils
  └── CSVExporter  (static, no DB dependency)
```

**Layer responsibilities:**
- `main.cpp` — entry point; wires DB path and launches window
- `UI` — all Qt widgets; calls `DatabaseManager` directly, no intermediate service layer
- `DatabaseManager` — singleton; owns the SQLite connection; all SQL lives here
- `Models` — plain data holders with typed accessors; no Qt parent hierarchy
- `Utils` — stateless helpers; depend only on models

---

## Build & test

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)
cd build && ctest --output-on-failure   # requires Qt6::Test
./build_installer.sh                    # produces .deb
```

## Common tasks

| Task | Where to make the change |
|---|---|
| Add a field to a transaction | `Transaction` model → `DatabaseManager` SQL → `AddTransactionDialog` form → `MainWindow::refreshTable` |
| Add a chart | New `QChartView*` factory in `DashboardWidget`; query data via `DatabaseManager` |
| Add an export format | New static class in `src/utils/`, mirroring `CSVExporter` |
| Change the DB schema | Bump `PRAGMA user_version` in `DatabaseManager::createTables`; add migration block in `initialize()` |
| Add a new category | Extend the combo box in `AddTransactionDialog::AddTransactionDialog` |

## Constraints

- UI widgets call `DatabaseManager` directly — there is no service layer
- All SQL must stay inside `DatabaseManager`; UI never builds queries
- `DatabaseManager` is a singleton; obtain it with `DatabaseManager::instance()`
- The transaction `id` stored in column 0's `Qt::UserRole` is the only link between a table row and the DB record

## Code conventions

- No comments unless the *why* is non-obvious
- When overriding Qt virtual methods, refer to the parent-class doc rather than restating the contract
