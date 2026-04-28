# Database

Single file: `DatabaseManager` — a singleton that owns the SQLite connection.

## Initialization

Call `DatabaseManager::instance().initialize(path)` once at startup before any other method. It opens the database, calls `createTables()`, and runs any pending schema migrations keyed on `PRAGMA user_version`.

## Schema

Single table: `transactions`

| Column | Type |
|---|---|
| `id` | INTEGER PRIMARY KEY AUTOINCREMENT |
| `title` | TEXT |
| `amount` | REAL |
| `type` | TEXT (`"INCOME"` / `"EXPENSE"`) |
| `category` | TEXT |
| `date` | TEXT (ISO 8601: `yyyy-MM-dd`) |

`Account` and `Budget` are not persisted — see [models README](../models/README.md).

## Adding a query

Add a method to `DatabaseManager` — never build SQL in UI code. Use `QSqlQuery::bindValue` for all user-supplied values to prevent injection.

## Schema migrations

Bump `PRAGMA user_version` in `createTables()` and add a versioned `if (version < N)` block in `initialize()` before the `createTables()` call.
